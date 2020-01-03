#!/bin/sh

# PREPARE SERIAL
stty -F /dev/ttyATH0 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts

# function send_to_powerBI
function send_to_powerBI_moeskast {
  REST_API_URL="https://api.powerbi.com/beta/fea0c9e8-3063-4898-88f8-cae45faaa84d/datasets/292d2963-2e0d-47ed-9412-5fd05170091b/rows?key=nQUflvdmcJsTmzeXr5UQ1Bll%2B0EiJQiCjCTZy6boSHmmvj1oL%2FASydrRCFfesBr886B207%2B8NVDuieE%2FqfYQGw%3D%3D"
  # info on the command line args:
  # -s == silient
  # -S == echo any ERRORS to the terminal
  curl -s -S --request POST --header "Content-Type: application/json" --data-binary "[${1}]" $REST_API_URL
}

function send_to_powerBI_feinstaub {
  REST_API_URL="https://api.powerbi.com/beta/fea0c9e8-3063-4898-88f8-cae45faaa84d/datasets/43aa2eca-2998-42e8-88dc-052f191bce61/rows?key=cC06gPAuOcmVBZJFxVO6Z354MAdvLg2W1HeEyLtu7Yum138L%2BW5MpCYZQZfLYeCXnO%2BmF%2F0lKKR2KlPS9SWbgA%3D%3D"
  # info on the command line args:
  # -s == silient
  # -S == echo any ERRORS to the terminal
  FEINSTAUB_API_URL="http://192.168.1.25/data.json"
  response=$(curl -s -S --request POST --header "Content-Type: application/json" $FEINSTAUB_API_URL)
  PM10=$(echo $response | jq -r '.sensordatavalues[0].value')
  PM25=$(echo $response | jq -r '.sensordatavalues[1].value')
  BTEMP=$(echo $response | jq -r '.sensordatavalues[2].value')
  data=$(echo "{\"date\":\"${1}\",\"PM10\":\"${PM10}\",\"PM25\":\"${PM25}\",\"BTEMP\":\"${BTEMP}\"}")
  curl -s -S --request POST --header "Content-Type: application/json" --data-binary "[$data]" $REST_API_URL
}

# function locateValue
function locateValue {
   grep -m 1 $1 | sed -n "s/^.*\(""$1""[^""$2""]*\).*/\1/p" | sed "s/""$1""//"
}

# function json_value
function get_json_value () {
    key=$1
    json=$2
    result=$(echo $json |jq -j .${key} )
    if [ ! "$result" == "null" ]; then
        echo $result
    else
        echo "NotAvailable"
    fi
}

# gpio export & status
function gpiostatus {
STATUS_GPIO0=$($SCRIPTPATH 0);
STATUS_GPIO1=$($SCRIPTPATH 1);
STATUS_GPIO6=$($SCRIPTPATH 6);
STATUS_GPIO7=$($SCRIPTPATH 7);
STATUS_GPIO8=$($SCRIPTPATH 8);
STATUS_GPIO14=$($SCRIPTPATH 14);
STATUS_GPIO16=$($SCRIPTPATH 16);
}

# functiona main
function main {

# date +%D	Displays date as mm/dd/yy format
# date +%H	Displays hour in (00..23) format 
# date +%M	Displays minute (00..59)
# date +%S	Displays second (00..60)
# date +%T	Displays time; shown as HH:MM:SS
# date +%u	Displays day of week (1..7); 1 is Monday

# get hour, minute & second
#TIMESTAMP=use coreutils-date
NSec=$(/usr/bin/date +%N | awk '{x=$1}END{print int(x/1000000)}')
TIMESTAMP=$(/usr/bin/date "+%Y-%m-%dT%H:%M:%S")"."${NSec}"Z"
DATE=$(date +%D)
HOUR=$(date +%H)
MINUTE=$(date +%M)
MINUTES=$(awk -v hour=$HOUR -v minute=$MINUTE 'BEGIN{print (hour * 60) + minute}')
SECONDS=$(date +%S)

# get temperature from weatherstationamsterdam.nl
URL="http://weatherstationamsterdam.nl/weather28/uploadMB/tagsMB.txt"
AMSDATA=$(curl -s -N $URL)
AMSTEMP=$(echo $AMSDATA | locateValue "tempAct|" "|")
AMSHUM=$(echo $AMSDATA | locateValue "humiAct|" "|")
AMSBAR=$(echo $AMSDATA | locateValue "baroAct|" "|")

# get full line of data from sensorstation Arduino
arduinoData=$(cat </dev/ttyATH0 | grep -m 1 "T_BME680")

# First check if the $arduinoData contains valid json
if [ $(get_json_value "EC" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "DS18B20" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "D_US100" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "T_US100" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "T_BME680" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "T_BME680DEC0" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "P_BME680" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "H_BME680" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "G_BME680" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "T_MTH02" "$arduinoData") == "NotAvailable" ] ||
	[ $(get_json_value "H_MTH02" "$arduinoData") == "NotAvailable" ]; then
    arduinoData="invalid";
fi

#specify script path
SCRIPTNAME="gpiocontrol.sh"
SCRIPTPATH="/mnt/sda1/moeskast/gpio/"$SCRIPTNAME

# gpio export & status
gpiostatus;

#GPIO 0 ON: Four IR panels
if [ $arduinoData != "invalid" ]; then
    if [ "${STATUS_GPIO0}" -eq 0 ]; then
        if [ "${T_BME680DEC0}" -lt 1500 ]; then $SCRIPTPATH 0 out 1;fi
    else # --> GPIO 0 OFF
        if [ "${T_BME680DEC0}" -gt 1800 ]; then $SCRIPTPATH 0 out 0;fi
    fi
fi


#GPIO 1 ON: Low noise Fan
if [ $arduinoData != "invalid" ]; then
	if [ "${STATUS_GPIO1}" -eq 0 ]; then
		if [ "${T_BME680DEC0}" -gt 2200 ]; then $SCRIPTPATH 1 out 1;fi
	else # --> GPIO 1 OFF
		if [ "${T_BME680DEC0}" -lt 2000 ]; then $SCRIPTPATH 1 out 0;fi
	fi
fi


#GPIO 6 ON: 2 luminaires (2/6)
if [ "${STATUS_GPIO6}" -eq 0 ]; then 
	if [ "${HOUR}" -ge 18 ] || [ "${HOUR}" -lt 2 ]; then $SCRIPTPATH 6 out 1;fi
else # --> GPIO 6 OFF
	if [ "${HOUR}" -ge 2 ] && [ "${HOUR}" -lt 18 ]; then $SCRIPTPATH 6 out 0;fi
fi


#GPIO 7 ON: 2 luminaires (4/6)
if [ "${STATUS_GPIO7}" -eq 0 ]; then 
	if [ "${HOUR}" -ge 2 ] && [ "${HOUR}" -lt 10 ]; then $SCRIPTPATH 7 out 1;fi
else # --> GPIO 7 OFF
	if [ "${HOUR}" -ge 10 ] || [ "${HOUR}" -lt 2 ]; then $SCRIPTPATH 7 out 0;fi
fi


#GPIO 8 ON:  2 luminaires (6/6)
if [ "${STATUS_GPIO8}" -eq 0 ]; then 
	if [ "${HOUR}" -ge 22 ] || [ "${HOUR}" -lt 6 ]; then $SCRIPTPATH 8 out 1;fi
else # --> GPIO 8 OFF
	if [ "${HOUR}" -ge 6 ] && [ "${HOUR}" -lt 22 ]; then $SCRIPTPATH 8 out 0;fi
fi


#GPIO 14 ON:  Pump + Ozon, 4 moments (0, 6, 12 and 18 hrs) the pump turns on untill the next cron cycle
if [ "${STATUS_GPIO14}" -eq 0 ]; then
	if [ "${MINUTES}" -ge 0 ] && [ "${MINUTES}" -lt 15 ]; then $SCRIPTPATH 14 out 1;fi
	if [ "${MINUTES}" -ge 360 ] && [ "${MINUTES}" -lt 375 ]; then $SCRIPTPATH 14 out 1;fi
	if [ "${MINUTES}" -ge 720 ] && [ "${MINUTES}" -lt 735 ]; then $SCRIPTPATH 14 out 1;fi
	if [ "${MINUTES}" -ge 1080 ] && [ "${MINUTES}" -lt 1095 ]; then $SCRIPTPATH 14 out 1;fi
else # --> GPIO 14 OFF
	if [ "${MINUTES}" -ge 15 ] && [ "${MINUTES}" -lt 360 ]; then $SCRIPTPATH 14 out 0;fi
	if [ "${MINUTES}" -ge 375 ] && [ "${MINUTES}" -lt 720 ]; then $SCRIPTPATH 14 out 0;fi
	if [ "${MINUTES}" -ge 735 ] && [ "${MINUTES}" -lt 1080 ]; then $SCRIPTPATH 14 out 0;fi
	if [ "${MINUTES}" -ge 1095 ]; then $SCRIPTPATH 14 out 0;fi
fi


#GPIO 16 ON: Main water pump
if [ "${STATUS_GPIO16}" -eq 0 ]; then 
	$SCRIPTPATH 16 out 1;
fi


# gpio export & status
gpiostatus;

dptData=$(echo "{\"TIME\":\"${TIMESTAMP}\",\"AMSTEMP\":${AMSTEMP},\"AMSHUM\":${AMSHUM},\"AMSBAR\":${AMSBAR},\"IR\":${STATUS_GPIO0},\"FAN\":${STATUS_GPIO1},\"LIGHT\":${STATUS_GPIO6}${STATUS_GPIO7}${STATUS_GPIO8},\"OZON\":${STATUS_GPIO14},\"PUMP\":${STATUS_GPIO16}}")


# send data to PowerBImoeskast
if [ $arduinoData != "invalid" ]; then
	powerBiDataSet=$(echo $arduinoData | jq ". += $dptData")
	echo $powerBiDataSet
	send_to_powerBI_moeskast "${powerBiDataSet}"
fi

# send data to powerBI_feinstaub
send_to_powerBI_feinstaub "${TIMESTAMP}"

# end of function main
}

# execute the main function and loop
while :; do sleep 1m & main; wait; done

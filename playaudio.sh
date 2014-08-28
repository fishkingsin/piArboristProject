#!/bin/sh

SERVICE='mpg321'
while true; do
if ps ax | grep -v grep | grep $SERVICE > /dev/null
then
echo " " 
# sleep 1
else
mpg321 $1 &
fi
done

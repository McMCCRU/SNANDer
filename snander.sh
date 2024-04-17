#!/bin/bash
#BASH SCRIPT FOR PALYING SNANDer.sh By Mikhail Medvedev
# https://github.com/bigbigmdm/bash-for-SNANDer
t1="24Cxx - Two-Wire Serial EEPROM"
t2="93Cxx - MICROWIRE Serial EEPROM"
ch_dev=$(lsusb | grep "1a86:" -o)
echo $ch_dev
if [[ "$ch_dev" == "1a86:" ]]
then
epromtype=$(zenity --height=300 --width=300 --list --radiolist --text \
"Выберите тип EEPROM:" --column="Set" --column="EEPROM type"\
 FALSE "24Cxx - Two-Wire Serial EEPROM"\
 FALSE "93Cxx - MICROWIRE Serial EEPROM"\
 FALSE "25Cxx - NAND FLASH EEPROM"\
 FALSE "25Qxx - NOR FLASH EEPROM"\ )
 if [ -n "$epromtype" ]
 then
     if [[ "$epromtype" == "24Cxx - Two-Wire Serial EEPROM" ]]
     then
     eeprom_model=$(zenity --height=300 --width=300 --list --radiolist --text \
"24Cxx - Two-Wire Serial EEPROM:" --column="Set" --column="EEPROM model"\
 TRUE "24c01"\
 FALSE "24c02"\
 FALSE "24c04"\
 FALSE "24c16"\
 FALSE "24c32"\
 FALSE "24c64"\
 FALSE "24c128"\
 FALSE "24c256"\
 FALSE "24c512"\ )
     fi
     if [[ "$epromtype" == "93Cxx - MICROWIRE Serial EEPROM" ]]
     then
     eeprom_model=$(zenity --height=300 --width=300 --list --radiolist --text \
"24Cxx - Two-Wire Serial EEPROM:" --column="Set" --column="IC model"\
 TRUE "93c06"\
 FALSE "93c16"\
 FALSE "93c46"\
 FALSE "93c56"\
 FALSE "93c66"\
 FALSE "93c76"\
 FALSE "93c86"\
 FALSE "93c96"\ )
     fi
     action_type=$(zenity --height=340 --width=300 --list --radiolist --text \
"Please select the action:" --column="Set" --column="Action"\
 TRUE "Reading"\
 FALSE "Writing"\
 FALSE "Erasing" )
     if [[ "$action_type" == "Reading" ]]
     then
     zenity --warning \
--text="Please select the storing directory"
     filepath=$(zenity --file-selection --directory)
     filename=$(zenity --entry \
--title="Enter the name of file" \
--text="Name of file to saving:" \
--entry-text "eeprom.bin")
        if [ -n "$eeprom_model" ]
        then
        SNANDer -E$eeprom_model -r $filepath/$filename | tee >(zenity --width=200 --height=100 \
  				    --title="Reading" --progress \
			            --pulsate --text="Please wait..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        else
        SNANDer -r $filepath/$filename | tee >(zenity --width=200 --height=100 \
  				    --title="Reading" --progress \
			            --pulsate --text="Please wait" \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        fi
     fi
     if [[ "$action_type" == "Writing" ]]
     then
     zenity --warning \
--text=""
     filename=$(zenity --file-selection)     
        if [ -n "$eeprom_model" ]
        then
        SNANDer -E$eeprom_model -w $filename | tee >(zenity --width=200 --height=100 \
  				    --title="Writing" --progress \
			            --pulsate --text="Please wait..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        else
        SNANDer -w $filename | tee >(zenity --width=200 --height=100 \
  				    --title="Writing" --progress \
			            --pulsate --text="Please wait..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        fi
     fi
     if [[ "$action_type" == "Erasing" ]]
     then   
        if [ -n "$eeprom_model" ]
        then
        SNANDer -E$eeprom_model -e | tee >(zenity --width=200 --height=100 \
  				    --title="Erasing" --progress \
			            --pulsate --text="Please wait..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        else
        SNANDer -e | tee >(zenity --width=200 --height=100 \
  				    --title="Erasing" --progress \
			            --pulsate --text="Please wait..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        fi
     fi
     zenity --error \
--text="Success"
 else 
 echo "The chip is not selected!"
 zenity --error \
--text="The chip is not selected!"
 fi   
else
 zenity --error \
--text="The device CH341A is not found!"
 fi

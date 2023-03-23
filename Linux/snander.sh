#!/bin/bash
t1="24Cxx - Two-Wire Serial EEPROM"
t2="93Cxx - MICROWIRE Serial EEPROM"
ch_dev=$(lsusb | grep "1a86:" -o)
echo $ch_dev
if [[ "$ch_dev" == "1a86:" ]]
then
epromtype=$(zenity --height=320 --width=320 --list --radiolist --text \
"Выберите тип EEPROM:" --column="Set" --column="Тип микросхемы"\
 FALSE "24Cxx - Two-Wire Serial EEPROM"\
 FALSE "93Cxx - MICROWIRE Serial EEPROM"\
 FALSE "25Cxx - NAND FLASH EEPROM"\
 FALSE "25Qxx - NOR FLASH EEPROM")
 if [ -n "$epromtype" ]
 then
     if [[ "$epromtype" == "24Cxx - Two-Wire Serial EEPROM" ]]
     then
     echo "24Cxx selected"
     eeprom_model=$(zenity --height=330 --width=320 --list --radiolist --text \
"24Cxx - Two-Wire Serial EEPROM:" --column="Set" --column="Модель микросхемы"\
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
     echo "93Cxx selected"
     eeprom_model=$(zenity --height=320 --width=320 --list --radiolist --text \
"93Cxx - Microwire EEPROM:" --column="Set" --column="Модель микросхемы"\
 TRUE "93c06"\
 FALSE "93c16"\
 FALSE "93c46"\
 FALSE "93c56"\
 FALSE "93c66"\
 FALSE "93c76"\
 FALSE "93c86"\
 FALSE "93c96"\ )
     fi
     if [ "$epromtype" == "25Cxx - NAND FLASH EEPROM" ] || [ "$epromtype" == "25Qxx - NOR FLASH EEPROM" ]
     then
        echo "NOR or NAND selected"
        manufacture='Производитель: '
        info=$(SNANDer -i)
        if [[ $info == *"id: 7f"* ]]
          then
          zenity --warning \
          --text="Пожалуйста подключите адаптер на 1,8 вольт!"
          fi
        man_code=$(echo "$info" | grep 'spi device id:')
        dev_vcc=$(echo "$info" | grep 'VCC:')
        if [[ $man_code == *"spi device id:"* ]] 
          then
             if [[ $man_code == *"id: 01"* ]]; then man_name="SPANSION"; fi
             if [[ $man_code == *"id: 0b"* ]]; then man_name="XTX"; fi
             if [[ $man_code == *"id: 1c"* ]]; then man_name="Eon"; fi
             if [[ $man_code == *"id: 1f"* ]]; then man_name="Atmel"; fi
             if [[ $man_code == *"id: 20"* ]]; then man_name="Micron"; fi
             if [[ $man_code == *"id: 5e"* ]]; then man_name="Zbit"; fi
             if [[ $man_code == *"id: 68"* ]]; then man_name="Boya"; fi
             if [[ $man_code == *"id: 7f"* ]]; then man_name="ISSI"; fi
             if [[ $man_code == *"id: 85"* ]]; then man_name="PUYA"; fi
             if [[ $man_code == *"id: 9d"* ]]; then man_name="ISSI"; fi
             if [[ $man_code == *"id: a1"* ]]; then man_name="Fudan"; fi
             if [[ $man_code == *"id: ba"* ]]; then man_name="Zetta"; fi
             if [[ $man_code == *"id: c2"* ]]; then man_name="MXIC (Macronix)"; fi
             if [[ $man_code == *"id: c8"* ]]; then man_name="GigaDevice"; fi
             if [[ $man_code == *"id: e0"* ]]; then man_name="PARAGON"; fi
             if [[ $man_code == *"id: ef"* ]]; then man_name="Winbond"; fi   
             if [[ $man_code == *"id: f8"* ]]; then man_name="Fidelix"; fi   
          else  manufacture="Производитель не найден."
       fi;
       manufacture="${manufacture}<b>${man_name}</b>"
       info=$(echo "$info" | grep 'Flash:')
       rufirst="Найдена микросхема "
       rusecond="Объем: "
       ruvcc="Напряжение питания:"
       dev_vcc=${dev_vcc/'VCC:'/"$ruvcc <b>"}
       ruinfo=${info/'Detected '/"\n$rufirst"}
       ruinfo=${ruinfo/'Flash Size:'/"\n$rusecond"}
       ruinfo=${ruinfo/'[93m'/"<b>"}
       ruinfo=${ruinfo/'[93m'/"<b>"}
       ruinfo=${ruinfo/'[0m'/"</b>"}
       ruinfo=${ruinfo/'[0m'/"</b>"}
       ruinfo="${ruinfo} \n${manufacture}\n${dev_vcc}</b>"   
     fi
     action_type=$(zenity --height=340 --width=320 --list --radiolist --text \
"Выберите действие:" --column="Set" --column="Действие"\
 TRUE "Считать"\
 FALSE "Записать"\
 FALSE "Стереть" )
     if [[ "$action_type" == "Считать" ]]
     then
     zenity --warning --width=320 \
--text="Выберите каталог для сохранения файла"
     filepath=$(zenity --file-selection --directory)
     filename=$(zenity --entry \
--title="Введите имя файла" \
--text="Имя файла для сохранения:" \
--entry-text "eeprom.bin")
        if [ -n "$eeprom_model" ]
        then        
        SNANDer -E$eeprom_model -r $filepath/$filename | tee >(zenity --width=200 --height=100 \
  				    --title="Считывание" --progress \
			            --pulsate --text="Подождите, процесс выполняется..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        else
        SNANDer -r $filepath/$filename | tee >(zenity --width=320 --height=100 \
  				    --title="Считывание" --progress \
			            --pulsate --text="$ruinfo\n\nПодождите, процесс выполняется..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        fi
     fi
     if [[ "$action_type" == "Записать" ]]
     then
     zenity --warning \
--text="Выберите файл для записи микросхемы"
     filename=$(zenity --file-selection)     
        if [ -n "$eeprom_model" ]
        then
        SNANDer -E$eeprom_model -w $filename | tee >(zenity --width=200 --height=100 \
  				    --title="Запись" --progress \
			            --pulsate --text="Подождите, процесс выполняется..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        else
        SNANDer -w $filename | tee >(zenity --width=320 --height=100 \
  				    --title="Запись" --progress \
			            --pulsate --text="$ruinfo\n\nПодождите, процесс выполняется..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        fi
     fi
     if [[ "$action_type" == "Стереть" ]]
     then   
        if [ -n "$eeprom_model" ]
        then
        SNANDer -E$eeprom_model -e | tee >(zenity --width=200 --height=100 \
  				    --title="Стирание" --progress \
			            --pulsate --text="Подождите, процесс выполняется..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        else
        SNANDer -e | tee >(zenity --width=320 --height=100 \
  				    --title="Стирание" --progress \
			            --pulsate --text="$ruinfo\n\nПодождите, процесс выполняется..." \
                                    --auto-kill --auto-close \
                                    --percentage=10)
        fi
     fi
     zenity --warning \
--text="Готово" \
--icon-name='applications-electronics'
 else 
 echo "Микросхема не выбрана"
 zenity --error \
--text="Микросхема не выбрана"
 fi
 else
 zenity --error \
--text="Программатор CH341A не подключен!"
 fi


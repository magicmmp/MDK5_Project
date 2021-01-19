#!/bin/sh

git add *

date_time=$(date)

git commit -m "auto git push  MDK5 project of stm32F103: ${date_time}"
#git commit -m "初步完成p32_SYN480R_decode项目,解码无线模块SYN480R。配套发射信号项目p32_SYN480R_SendSignal_Taobao2014: ${date_time}"
git push origin master


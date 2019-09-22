#!/bin/sh

git add *

date_time=$(date)

git commit -m "auto git push  MDK5 project of stm32F103: ${date_time}"

git push origin master


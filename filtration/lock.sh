#!/bin/bash

cd ~/dotfiles/filtration && ./filtration -b 14 -t test.png -s med-res-lock.png
i3lock -u -i test.png

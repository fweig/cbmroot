#!/bin/bash
# Copyright (C) 2020 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

pkill -SIGINT Hit
pkill -SIGINT Unp
sleep 30
pkill -9 Tsa
pkill -9 Hit
pkill -9 Unp
pkill -9 parmq

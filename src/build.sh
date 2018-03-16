#!/bin/bash


gcc main.c packcap.c ether_handle.c ipvx_handle.c tcp_handle.c -lpcap

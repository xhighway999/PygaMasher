project(fec)
cmake_minimum_required(VERSION 2.6)

add_library(fec 
	fec.h
	fec.c
	encode_rs_char.c
	decode_rs_char.c
	init_rs_char.c
	)


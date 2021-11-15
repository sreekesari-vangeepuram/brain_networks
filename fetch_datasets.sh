#!/bin/bash

# Create a directory to store datasets
mkdir datasets/

# Fetch all the datasets from the links provided in <links.txt>
wget --directory-prefix datasets/ --input-file links.txt

# Extract all the files downloaded in compressed format
for file in ./datasets/*
do
	unzip $file -d $(basename $file .zip)
done

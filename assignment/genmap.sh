#!/bin/bash


# Write a shell script to generate (pseudo)-random rectangular ASCII maps. Call the script genmap.sh.
# You may adapt gen food() from the example snake housenka.sh game with appropriate credits

# The script should have default map dimensions set at 50 × 50 characters
width=50
height=50


# Allow those to be overridden by one or two command line arguments. 
if [[ $# -eq 1 ]]; then
    width=$1
elif [[ $# -eq 2 ]]; then
    width=$1
    height=$2
fi

for (( try=0; try<10; try++ )); do
	map=""
	total=0
	for (( i=0; i<$height; i++ )); do
		for (( j=0; j<$width; j++ )); do
			total=$((total+1))
			if [[ $(( RANDOM % 2 )) -eq 0 ]]; then
				map+=" "
			else
				map+="#"
			fi
		done
		map+="\n"
	done
	
	# Count the number of non blank chars
	non_blank=$(echo -e $map | tr -d -c "#" | wc -c)
	blank=$(echo -e $map | tr -d -c " " | wc -c)
	echo "non_blank: $non_blank, blank: $blank"
	# if the map is more than 50% non-blank, then we are done
	if [[ $non_blank -gt $blank ]]; then
		echo -e $map
		exit 0
	fi
done
# Only output the map to final STDOUT of it meets this requirement. If you are
# not successful meeting this requirements after 10 iterations, then exit with an error message to
# STDERR informing the script user of the fact.
echo "Could not generate map" >&2
exit 1
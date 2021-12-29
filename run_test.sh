#!/bin/bash
echo "Compile Algorithms"
g++ -g -O2 -std="c++17" -static -o ./code/ida_star.out ./code/ida_star.cpp
g++ -g -O2 -std="c++17" -static -o ./code/classic_a_star.out ./code/classic_a_star.cpp
g++ -g -O2 -std="c++17" -static -o ./code/ida_star_with_memorization.out ./code/ida_star_with_memorization.cpp
g++ -g -O2 -std="c++17" -static -o ./code/persistent_a_star.out ./code/persistent_a_star.cpp

echo "Config limits: 16 GB of memory and 60 seconds"
ulimit -d 16777216
ulimit -m 16777216
ulimit -v 16777216
ulimit -t 60

echo "Run programming"
echo '' > output_test.txt

for file in ./test/*; do
	printf "$file:\n" >> output_test.txt

	printf "ida_star: " >> output_test.txt
  ./code/ida_star.out < "$file" >> output_test.txt
	printf "\n" >> output_test.txt

	printf "classic_a_star: " >> output_test.txt
	./code/classic_a_star.out < "$file" >> output_test.txt
	printf "\n" >> output_test.txt

	printf "ida_star_with_memorization: " >> output_test.txt
	./code/ida_star_with_memorization.out < "$file" >> output_test.txt
	printf "\n" >> output_test.txt

	printf "persistent_a_star: " >> output_test.txt
	./code/persistent_a_star.out < "$file" >> output_test.txt
	printf "\n" >> output_test.txt

	printf "\n" >> output_test.txt
done

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

int main() {
    // Open the input file
    std::ifstream input("input.txt");
    if (!input) {
        std::cerr << "Error: Could not open input.txt\n";
        return 1;
    }

    // Initialize the sum of calibration values
    int calibration_sum{};

    // Read each line from the input file
    std::string line;
    while (getline(input, line)) {
        // Find the first and last digit in the line
        int first = -1, last = -1;
        for (int i = 0; i < line.length(); i++) {
            // Check if the character is a digit
            if (std::isdigit(line[i])) {
                // Update the first and last digit
                if (first == -1) { // only gets executed the first time a digit is found
                    first = line[i] - '0'; // if we have, say, '6', then that is equivalent to 54, and 54 - 48 (48 being the equivalent to '0'), so we get 6 -> '6' to 6 haha
                }
                last = line[i] - '0'; // executed every time a digit is found in the line, so that we can actually get the last, and not stop at some middle value
            }
        }

        // Calculate the calibration value as the concatenation of the first and last digit
        int cal = first * 10 + last; // if first is 2, and last is 7, we'd like to have 27, and that's why 2*10+7 would give us that.

        // Add the calibration value to the sum
        calibration_sum += cal;
    }

    // Close the input file
    input.close();

    // Print the sum of calibration values
    std::cout << "The sum of calibration values is " << calibration_sum << "\n";

    return 0;
}

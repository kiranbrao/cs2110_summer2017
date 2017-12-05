/**
 * CS 2110 Summer 2017 HW2
 * Part 2 - Coding with bases
 *
 * @author Kiran Rao
 *
 * Global rules for this file:
 * - You may not use more than 2 conditionals per method. Conditionals are
 *   if-statements, if-else statements, or ternary expressions. The else block
 *   associated with an if-statement does not count toward this sum.
 * - You may not use more than 2 looping constructs per method. Looping
 *   constructs include for loops, while loops and do-while loops.
 * - You may not use nested loops.
 * - You may not declare any file-level variables.
 * - You may not declare any objects, other than String in select methods.
 * - You may not use switch statements.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any other method from this or
 *   another file to implement any method.
 * - The only Java API methods you are allowed to invoke are:
 *     String.length()
 *     String.charAt()
 * - You may not invoke the above methods from string literals.
 *     Example: "12345".length()
 * - When concatenating numbers with Strings, you may only do so if the number
 *   is a single digit.
 *
 * Method-specific rules for this file:
 * - You may not use multiplication, division or modulus in any method, EXCEPT
 *   decimalStringToInt.
 * - You may declare exactly one String variable each in intToBinaryString and
 *   and intToHexString.
 */
public class Bases
{
    /**
     * Convert a string containing ASCII characters (in binary) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid binary numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: binaryStringToInt("111"); // => 7
     */
    public static int binaryStringToInt(String binary)
    {
        int output = 0;
        for(int i=0; i<binary.length(); i++){
            if (binary.charAt(binary.length()-i-1) == (char)('1')) {
                output += 1 << i;
            }
        }
        return output;
    }

    /**
     * Convert a string containing ASCII characters (in decimal) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid decimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: decimalStringToInt("123"); // => 123
     */
    public static int decimalStringToInt(String decimal)
    {
        int output = 0;
        for(int i=0; i<decimal.length(); i++) {
            int place = decimal.charAt(i) - '0';
            output = output*10 + place;
        }
        return output;
    }

    /**
     * Convert a string containing ASCII characters (in hex) to an int.
     * The input string will only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid hexadecimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: hexStringToInt("A6"); // => 166
     */
    public static int hexStringToInt(String hex)
    {
        int output = 0;
        for(int i=0, j=0; i<hex.length(); i++, j=j+4){
            if (hex.charAt(hex.length()-i-1) >= '0' && hex.charAt(hex.length()-i-1) <= '9') {
                int place = hex.charAt(hex.length()-i-1) - '0';
                output += place << j;
            } else {
                int place = hex.charAt(hex.length()-i-1) - 'A' + 10;
                output += place << j;
            }
        }
        return output;
    }

    /**
     * Convert a int into a String containing ASCII characters (in binary).
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToBinaryString(7); // => "111"
     */
    public static String intToBinaryString(int binary)
    {
        String returnString = "";
        long comparison = 0;
        int count = 0;
        while(binary > comparison) {
            comparison = 1L << count+1;
            count++;
        }
        if(count == 0) {
            count++;
        }
        for(int i = 0; i<count; i++){
            if ((binary>>i & (1<<0)) != 0) {
                returnString = "1" + returnString;
            } else {
                returnString = "0" + returnString;
            }
        }
        return returnString;
    }

    /**
     * Convert a int into a String containing ASCII characters (in hexadecimal).
     * The output string should only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToHexString(166); // => "A6"
     */
    public static String intToHexString(int hex)
    {
        String returnString = "";
        int shiftCount = 0;
        int tempHex = hex;
        if (hex == 0) {
            return "0";
        }
        while ((tempHex & (1<<31)) == 0) {
            tempHex = tempHex << 1;
            shiftCount++;
        }
        int length = 32 - shiftCount;
        int currentFour = 0;
        for (int i = 4; i <= length + 3; i=i+4) {
            currentFour = ((hex >> (i-4)) & 0xF);
            if (currentFour < 10) {
                returnString = (char)('0' + currentFour) + returnString;
            } else {
                returnString = (char)('A' + (currentFour - 10)) + returnString;
            }
        }
        return returnString;
    }
}

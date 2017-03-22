Readme
------

Test files.
Test files MUST be created in CSV format.

Example:
--------

Message Type 1 – contains the details of 1 sale E.g apple at 10p
should be strictly entered in a new line as:
--------------
apple,10
--------------

Message Type 2 – contains the details of a sale and the number of occurrences of
that sale. E.g 20 sales of apples at 10p each.
should be strictly entered in a new line as:
--------------
apples,10,20
--------------

Message Type 3 – contains the details of a sale and an adjustment operation to be
applied to all stored sales of this product type. Operations can be add, subtract, or
multiply e.g Add 20p apples would instruct your application to add 20p to each sale
of apples you have recorded.
should be strictly entered in a new line as:
--------------
apples,10,20,ADD,30
--------------
where first 10 and 20 is the details of a sale and final value 30 is the adjustment

------------------------------------------------------------------------------------

To run the test, just compile the whole code with and c++14 compiler and run with one test file at a time.

SUNNY DAY scenario test file is added as sunny_day.txt by default.

Expecting strict test file creation, as due to time constraints, the invalid messages are not handled correctly.

FORMAT of each message in the test file:

apples,10,20,ADD,30

apples -->and product in string format.(String, mandatory )
10 --> price value in number (Unsigned long, Mandatory)
20 --> quantity in number (Unsigned long, optional)
ADD --> operation to be performed (Valid values: ADD,SUB,MUL) , case sensitive,optional
30 --> adjustment value(Unsigned long, optional, but must be provided if operation is specified, else message will be invalid)


To run a different test, change in the supporting function, the name of the file from "sunny_day.txt" to "testX.txt"
:
###################################################################
void build_message_queue(list<unique_ptr<message>>& message_list)
{
	ifstream infile("sunny_day.txt");
###################################################################

NOTE:For simplicity I am not including all te test files/UT files I used.
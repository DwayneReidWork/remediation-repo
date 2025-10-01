10 REM Simple BASIC program using core commands
20 PRINT "Welcome to the BASIC Calculator"
30 INPUT "Enter first number: ", A
40 INPUT "Enter second number: ", B
50 PRINT "Choose an operation:"
60 PRINT "1. Addition"
70 PRINT "2. Subtraction"
80 PRINT "3. Multiplication"
90 PRINT "4. Division"
100 INPUT "Enter choice (1-4): ", C

110 IF C = 1 THEN GOSUB 200
120 IF C = 2 THEN GOSUB 210
130 IF C = 3 THEN GOSUB 220
140 IF C = 4 THEN GOSUB 230
150 GOTO 240

200 REM Addition Subroutine
201 LET RESULT = A + B
202 PRINT "Result: "; RESULT
203 RETURN

210 REM Subtraction Subroutine
211 LET RESULT = A - B
212 PRINT "Result: "; RESULT
213 RETURN

220 REM Multiplication Subroutine
221 LET RESULT = A * B
222 PRINT "Result: "; RESULT
223 RETURN

230 REM Division Subroutine
231 IF B = 0 THEN PRINT "Error: Division by zero": RETURN
232 LET RESULT = A / B
233 PRINT "Result: "; RESULT
234 RETURN

240 PRINT "Thank you for using the calculator!"
250 END

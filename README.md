# RocketCasino
A console-based casino game ("Crash") inspired by popular online casinos such as Bustabit and Roobet.

![Example Image](https://ninja.photos/bW9DBum1TG-2309397429.png)

# How to Play
1) Compile Game
2) Run it
3) Enter your Starting Balance
4) Enter your bet amount (<= balance)
5) Enter an automatic cashout level above 1.00, or type 0/press enter to disable automatic cashout for this round
6) Wait for the game start
7) Press enter to cashout immediately. If you set an automatic cashout level, it will cashout at that point.
8) If the rocket crashes before you cashout, you lose your initial bet. If it doesn't, you increase your bet by the multiplier at which you cashed out.

# Game Formula
[Note: While Bustabit and Roobet use verifiable hashes based on SHA256 cryptography, RocketCasino emulates the results using a simple RNG.]
1) A number between 0 and 100 is generated. If it is 0, 1, or 2, the 'rocket' crashes immediately (1.0 is returned as the crashpoint).
2) A new number ("x") between 0 and 1000 is generated. It is divided by 1000 to obtain a double-value between 0 and 1. 
3) 99 is divided by 1-x
4) x is floored
5) x is again divided by 100. If the number is less than 1, 1 is used by default

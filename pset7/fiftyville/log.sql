-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1) We'll want to read the interviews from the day of the theft, to see what further information can be gleaned
SELECT name, year, month, day, transcript FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28
ORDER BY month;

-- 2) Eugene mentioned that, on the morning of the incident, he noticed the thief withdrawing money on Fifer street
-- (see interviews.txt for full text of 7/28/2020 interviews).
-- Now, we can look at who was withdrawing money
-- the morning of the 28th.
SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND transaction_type = "withdraw" AND atm_location = "Fifer Street";

-- 3) We can use the account numbers (given by the above query) and person_id in the bank accounts table to fetch the associated name in the people table.
-- To do this, we can use the above query as a subquery:
SELECT name, account_number FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND transaction_type = "withdraw" AND atm_location = "Fifer Street")
ORDER BY name;

-- 4) The above gives us a list of 8 names: Bobby, Danielle, Elizabeth, Ernest, Madison, Roy, Russel, Victoria
-- We also know that, according to Ruth, the thief was leaving the courthouse within 10 minutes of the theft.
-- So we can look at the courthouse records to see which cars were leaving that day
SELECT DISTINCT(license_plate) FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit"
ORDER BY license_plate

-- 5) AND we can crossreference license plates with names in the people table. We can do this using the above as a subquery:
SELECT name FROM people
WHERE license_plate IN
(SELECT DISTINCT(license_plate) FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit"
ORDER BY license_plate);

-- 6) We can use 5) and 3) to see which names appear in our ATM query and out courthouse query.
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE license_plate IN
(SELECT DISTINCT(license_plate) FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit")
AND account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND transaction_type = "withdraw" AND atm_location = "Fifer Street");

-- 7) This outputs: Ernest, Russell, Elizabeth, Danielle, Madison.
-- We can now return to basics and look at the initial crime scene reports
-- (something we should've done prior to looking at the interviews). This occurs to
-- me now because I notice that Ruth says the car left the court within 10 minutes of
-- the theft. And since I haven't learn the time of the theft, I figure I might find it in the crime scene report.
-- We'll want to see all information from the day of the theft on Chamberlain Street (the location of the theft)
-- We learn that the theft took place at 10:15 am. So we can run our above query again — this time with a specific time frame.
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE license_plate IN
(SELECT DISTINCT(license_plate) FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND hour = 10 AND minute >= 5 AND minute <= 25
AND activity = "exit")
AND account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND transaction_type = "withdraw" AND atm_location = "Fifer Street");

-- 8) The above produces 4 names (1 less than before): Ernest, Russell, Elizabeth, and Danielle. We also know that, according to Raymond,
-- the thief made a phone call while they were leaving the court house. The call lasted less than a minute. So we can
-- use the phone_calls table and the people table to find out who made a sub-minute phone call on July 28th, 2020.
SELECT name FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year == 2020 AND month = 7 AND day = 28
AND duration < 60);

-- 9) We can compare the resultant list produced by 8) to our list of 4 names to find which names appear in both lists:
SELECT name FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year == 2020 AND month = 7 AND day = 28
AND duration < 60) AND name IN
("Ernest", "Russell", "Elizabeth", "Danielle");

-- 10a) Now our list of suspects is down to 2 people: Russell and Ernest. Since Raymond mentioned that the thief made the phone call to purchase plane ticket
-- for the earliest flight out of fiftyville, we can query the flights table, passengers table and people table to find out who was on that flight. First
-- we can find out what the earliest flight out of Fiftyville is:
SELECT full_name, hour, minute FROM passengers
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2020 AND month = 7 AND day = 29
AND city = "Fiftyville"
ORDER BY hour;

-- 10b) It looks like the earliest flight is 8:20. So, now we can get a list of all the passengers on the 8:20 flights by using the
-- passport_numbers as our referenece.
SELECT name FROM people
WHERE passport_number IN
(SELECT passport_number FROM passengers
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2020 AND month = 7 AND day = 29
AND hour = 8 AND minute = 20
AND city = "Fiftyville");

-- 10c) Now, we can compare the list of names given by the above query to the list of 2 suspects.
SELECT name FROM people
WHERE passport_number IN
(SELECT passport_number FROM passengers
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2020 AND month = 7 AND day = 29
AND hour = 8 AND minute = 20
AND city = "Fiftyville")
AND name IN ("Russell", "Ernest");

-- 11) Finally, it looks like Ernest is our culprit, as he is the only suspect scheduled to take an 8:20 flight out of Fiftyville.
-- Now we need to find out where Ernest was going. To do this we can use the destination id in the flights table to
-- retrieve the city name in the airports table.
SELECT city FROM airports
WHERE id IN
(SELECT destination_airport_id FROM flights
WHERE year = 2020 AND month = 7 AND day = 29
AND hour = 8 AND minute = 20);

-- 12) It looks like he was going to London. Now we need to find out who the culprit is. To do this, we need to find out who
-- Ernest was calling from outside the court. We can do this by cross referencing the numbers in "people" with the numbers in
-- "phone_calls" where the caller was Ernest, the duration was less than 60 seconds, and the call occurred on July 28th, 2020.
SELECT name FROM people
WHERE phone_number IN
(SELECT receiver FROM phone_calls
WHERE caller =
(SELECT phone_number FROM people
WHERE name = "Ernest") AND duration < 60
AND year = 2020 AND month = 7 AND day = 28);

-- 13) We get 1 name, Berthold, who is our ACCOMPLICE
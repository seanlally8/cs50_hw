-- This file was used as "scratch paper" where I could test out queries without the clutter of log.sql
SELECT name FROM people
WHERE phone_number IN
(SELECT receiver FROM phone_calls
WHERE caller =
(SELECT phone_number FROM people
WHERE name = "Ernest") AND duration < 60
AND year = 2020 AND month = 7 AND day = 28);
SELECT title FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE name IN ("Johnny Depp", "Helena Bonham Carter")
GROUP BY title
HAVING COUNT(title) > 1;
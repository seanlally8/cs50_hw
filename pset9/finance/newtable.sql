CREATE TABLE purchases (
        trackuser INTEGER,
        num_of_shares INTEGER NOT NULL,
        stock_symbol TEXT NOT NULL,
        price NUMERIC NOT NULL,
        time_of_purchase NUMERIC NOT NULL,
        FOREIGN KEY(trackuser) REFERENCES users(id)
);

CREATE TABLE sales (
        trackuser INTEGER,
        num_of_shares INTEGER NOT NULL,
        stock_symbol TEXT NOT NULL,
        price NUMERIC NOT NULL,
        time_of_sale NUMERIC NOT NULL,
        FOREIGN KEY(trackuser) REFERENCES users(id)
);

CREATE INDEX sales_index
ON sales (
    trackuser,
    num_of_shares,
    stock_symbol,
    price,
    time_of_sale
);


CREATE INDEX purchase_index
ON purchases (
    trackuser,
    num_of_shares,
    stock_symbol,
    price,
    time_of_purchase
);

/* For index.html table */
SELECT stock_symbol, SUM(num_of_shares) AS num_of_shares, SUM(price) AS price
FROM purchases
WHERE trackuser = 1
GROUP BY stock_symbol;

SELECT num_of_shares FROM purchases WHERE trackuser = 1 AND stock_symbol = GOOG;

CREATE TABLE totals (
    trackuser INTEGER,
    stock_symbol TEXT NOT NULL,
    num_of_shares INTEGER NOT NULL,
    FOREIGN KEY(trackuser) REFERENCES users(id)
);

SELECT num_of_shares FROM totals WHERE trackuser = 1 AND stock_symbol = "NFLX";





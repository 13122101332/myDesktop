------------- ²»ÐÐ ----------------------
SELECT theTime=SUBSTRING(a, 1, 4), LAST_VALUE(a) OVER (PARTITION BY theTime ORDER BY a) AS c
 FROM (SELECT '1234' AS a) AS t
GO
SELECT theTime, LAST_VALUE(a) OVER (PARTITION BY theTime=SUBSTRING(a, 1, 4) ORDER BY a) AS c
 FROM (SELECT '1234' AS a) AS t
GO
---------------------------------------------

CREATE TABLE ##template_1minute (
	[time] VARCHAR(4),
	lowPrice SMALLMONEY,
	highPrice SMALLMONEY,
	closePrice SMALLMONEY,
	sumPrice DECIMAL(19, 3),
	num INT
)
CREATE TABLE ##template_15minute (
	[time] VARCHAR(4),
	lowPrice SMALLMONEY,
	highPrice SMALLMONEY,
	closePrice SMALLMONEY,
	sumPrice DECIMAL(19, 3),
	num INT
)
DROP TABLE ##template_15minute
DROP TABLE ##template_1minute

INSERT INTO ##template_15minute
SELECT RIGHT(CONCAT('0', theTime/4), 2)+RIGHT(CONCAT('0', theTime%4*15), 2) AS [time]
	, MIN(lowPrice) AS lowPrice, MAX(highPrice) AS highPrice, MAX(closePrice) AS closePrice
	, SUM(sumPrice*num) AS sumPrice, SUM(num) AS num
FROM(SELECT theTime=CONVERT(INT, SUBSTRING([time], 1, 2))*4+CONVERT(INT, SUBSTRING([time], 3, 2))/15
		, lowPrice, highPrice, sumPrice, num
		, LAST_VALUE(closePrice) OVER (PARTITION BY CONVERT(INT, SUBSTRING([time], 1, 2))*4+CONVERT(INT, SUBSTRING([time], 3, 2))/15 ORDER BY [time] ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING) AS [closePrice]
		FROM ##template_1minute) AS t
	GROUP BY theTime

INSERT INTO ##template_1minute SELECT theTime AS [time], MIN(price) AS lowPrice
	, MAX(price) AS highPrice, MAX(closePrice) AS [closePrice], SUM(CONVERT(DECIMAL(15, 3), price)*num) AS [sumPrice]
	, SUM(num) AS [num]
	 FROM (SELECT theTime=SUBSTRING([time], 1, 4)
		, closePrice=LAST_VALUE(price) OVER (PARTITION BY SUBSTRING([time], 1, 4) ORDER BY (id) ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING)
		, price, num FROM stock.dbo.tab_20150105SH600011) AS t
	GROUP BY theTime ORDER BY theTime ASC
	DELETE FROM ##template_1minute
SELECT * FROM ##template_1minute
SELECT * FROM ##template_15minute

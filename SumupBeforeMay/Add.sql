USE [stock_A]
GO

IF OBJECT_ID('to_temp_table', 'P') IS NOT NULL
	DROP PROC to_temp_table
GO
CREATE PROC to_temp_table(@code VARCHAR(6), @date_num VARCHAR(10))
AS
BEGIN
DELETE FROM ##temp_1minute
DECLARE @table_name VARCHAR(16)
SET @table_name = 's'+@code+'_'+@date_num
DECLARE @sql NVARCHAR(1024)
SET @sql = N'INSERT INTO ##temp_1minute SELECT theTime AS [time], MIN(price) AS lowPrice'
	+', MAX(price) AS highPrice, MAX(closePrice) AS [closePrice], SUM(price*num) AS [sumPrice]'
	+', SUM(num) AS [num] FROM (SELECT theTime=[time]/100'
	+'	, closePrice=LAST_VALUE(price) OVER (PARTITION BY [time]/100 ORDER BY (id) ASC ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING)'
	+'	, price, ABS(num) as num FROM stock_S.dbo.'+@table_name+') AS t'
	+' GROUP BY theTime'
EXEC sp_executesql @sql

DELETE FROM ##temp_15minute
INSERT INTO ##temp_15minute
SELECT theTime AS [time]
	, MIN(lowPrice) AS lowPrice, MAX(highPrice) AS highPrice, MAX(closePrice) AS closePrice
	, SUM(sumPrice) AS sumPrice, SUM(num) AS num
    FROM(SELECT [lowPrice], [highPrice], theTime=([time]/100)*100+(([time]%100)/15)*15, [sumPrice], [num], LAST_VALUE(closePrice) OVER (PARTITION BY ([time]/100)+([time]%100)/15 ORDER BY [time] ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING) AS [closePrice]
		FROM ##temp_1minute) AS t
	GROUP BY theTime
END
GO

IF OBJECT_ID('get_1day', 'P') IS NOT NULL
	DROP PROC get_1day
GO
CREATE PROC get_1day
AS
BEGIN
	SELECT MIN(lowPrice) AS lowPrice, MAX(highPrice) AS highPrice
		, MAX(closePrice) AS closePrice, SUM(1.0*sumPrice) AS sumPrice, SUM(num) AS num FROM (
	SELECT lowPrice, highPrice, sumPrice, num
		, LAST_VALUE(closePrice) OVER (ORDER BY [time] ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING) AS closePrice
		FROM ##temp_15minute) AS t
	RETURN;
END
GO

IF OBJECT_ID('d_1minute', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[d_1minute](
		[code] CHAR(6) NOT NULL,
		[date] [DATE] NOT NULL,
		[time] [TIME](0) NOT NULL,
		[lowPrice] [SMALLMONEY] NULL,
		[highPrice] [SMALLMONEY] NULL,
		[closePrice] [SMALLMONEY] NULL,
		[avgPrice] [SMALLMONEY] NULL,
		[num] [INT] NULL,
		[ema1] [SMALLMONEY] NULL,
		[ema2] [SMALLMONEY] NULL,
		[dif] [SMALLMONEY] NULL,
		[pole] [SMALLINT] NULL,
		[devia] [SMALLINT] NULL,
	PRIMARY KEY CLUSTERED 
	(
		[code] ASC,
		[date] ASC,
		[time] ASC
	)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
	) ON [PRIMARY]
END
GO
IF OBJECT_ID('d_15minute', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[d_15minute](
		[code] CHAR(6) NOT NULL,
		[date] [DATE] NOT NULL,
		[time] [TIME](0) NOT NULL,
		[lowPrice] [SMALLMONEY] NULL,
		[highPrice] [SMALLMONEY] NULL,
		[closePrice] [SMALLMONEY] NULL,
		[avgPrice] [SMALLMONEY] NULL,
		[num] [BIGINT] NULL,
		[ema1] [SMALLMONEY] NULL,
		[ema2] [SMALLMONEY] NULL,
		[dif] [SMALLMONEY] NULL,
		[pole] [SMALLINT] NULL,
		[devia] [SMALLINT] NULL,
	PRIMARY KEY CLUSTERED 
	(
		[code] ASC,
		[date] ASC,
		[time] ASC
	)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
	) ON [PRIMARY]
END
GO
IF OBJECT_ID('d_1day', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[d_1day](
		[code] CHAR(6) NOT NULL,
		[date] [DATE] NOT NULL,
		[time] [TIME](0) NULL,
		[lowPrice] [SMALLMONEY] NULL,
		[highPrice] [SMALLMONEY] NULL,
		[closePrice] [SMALLMONEY] NULL,
		[avgPrice] [SMALLMONEY] NULL,
		[num] [BIGINT] NULL,
		[ema1] [SMALLMONEY] NULL,
		[ema2] [SMALLMONEY] NULL,
		[dif] [SMALLMONEY] NULL,
		[pole] [SMALLINT] NULL,
		[devia] [SMALLINT] NULL,
	PRIMARY KEY CLUSTERED 
	(
		[code] ASC,
		[date] ASC
	)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
	) ON [PRIMARY]
END
GO

-------------------------------------------------------
USE stock_S
GO

IF OBJECT_ID('tabMgr', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[tabMgr](
		[date] [DATE] NULL,
		[code] CHAR(6) NULL,
		[log] VARCHAR(1024) NULL
	) ON [PRIMARY]
END
GO
IF OBJECT_ID('tabMgrHistory', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[tabMgrHistory](
		[date] [DATE] NULL,
		[code] CHAR(6) NULL,
		[log] VARCHAR(1024) NULL
	) ON [PRIMARY]
END
GO

IF OBJECT_ID('TempDataTable', 'U') IS NULL
BEGIN
	CREATE TABLE TempDataTable (code CHAR(6), type TINYINT, [date] DATE, [time] SMALLINT, lowPrice SMALLMONEY
		, highPrice SMALLMONEY, closePrice SMALLMONEY, num BIGINT, avgPrice SMALLMONEY
		, ema1 SMALLMONEY, ema2 SMALLMONEY, dif SMALLMONEY, prePrice SMALLMONEY
		, maxClosePrice SMALLMONEY, maxDif SMALLMONEY, maxDevia SMALLINT
		, minClosePrice SMALLMONEY, minDif SMALLMONEY, minDevia SMALLINT)
END 
GO

IF OBJECT_ID('Log', 'U') IS NULL
BEGIN
	CREATE TABLE Log (code CHAR(6), state VARCHAR(6), [bgDate] DATE, logName VARCHAR(50), logTime BIGINT)
	INSERT INTO Log VALUES('000000', 'FINISH', '2015-01-01', '', 0)
END









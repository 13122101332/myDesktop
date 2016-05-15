USE [stock_A]
GO

IF OBJECT_ID('create_template_table', 'P') IS NOT NULL
DROP PROC create_template_table
GO
CREATE PROC create_template_table
AS
BEGIN
	IF OBJECT_ID('tempdb..##template_1minute', 'U') IS NULL
	BEGIN
	CREATE TABLE ##template_1minute(
		[date] DATE,
		[time] TIME(0),
		lowPrice SMALLMONEY,
		highPrice SMALLMONEY,
		price SMALLMONEY,
		avgPrice SMALLMONEY,
		num INT
	)
	END
	IF OBJECT_ID('tempdb..##template_15minute', 'U') IS NULL
	BEGIN
	CREATE TABLE ##template_15minute(
		[date] DATE,
		[time] TIME(0),
		lowPrice SMALLMONEY,
		highPrice SMALLMONEY,
		price SMALLMONEY,
		avgPrice SMALLMONEY,
		num INT
	)
	END
END
GO

IF OBJECT_ID('to_temp_table_1minute', 'P') IS NOT NULL
	DROP PROC to_temp_table_1minute
GO
CREATE PROC to_temp_table_1minute(@table_name VARCHAR(30), @dateIn NVARCHAR(30))
AS
BEGIN
DECLARE @sql NVARCHAR(1024)
SET @sql = N'INSERT INTO ##template_1minute SELECT '''+@dateIn+''' AS [date], theTime AS [time]'
	+', MIN(price) AS lowPrice, MAX(price) AS highPrice, MAX(lastPrice) AS [price], SUM(1.0*price*num)/SUM(num) AS [avgPrice], SUM(num) AS [num] '
	+' FROM (SELECT theTime=SUBSTRING(CONVERT(varchar(20), tim, 8), 0, 6)  '
	+'	, lastPrice=LAST_VALUE(price) OVER (PARTITION BY DATEDIFF(MINUTE, ''00:00:00'', tim) ORDER BY (SELECT 0) ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING)'
	+'	, * FROM stock.dbo.'+@table_name+') AS t '
	+'GROUP BY theTime ORDER BY theTime ASC'
EXEC sp_executesql @sql
END
GO

IF OBJECT_ID('to_temp_table_15minute', 'P') IS NOT NULL
	DROP PROC to_temp_table_15minute
GO
CREATE PROC to_temp_table_15minute
AS
BEGIN
INSERT INTO ##template_15minute
SELECT MAX([date]) AS [date]
	, CONVERT(VARCHAR(20), DATEADD(MINUTE, theTime*15, '00:00:00'), 108) AS [time]
	, MIN(lowPrice) AS lowPrice
	, MAX(highPrice) AS highPrice, MAX(price) AS price, SUM(1.0*avgPrice*num)/SUM(num) AS avgPrice, SUM(num) AS num
FROM(SELECT [date], theTime=DATEDIFF(MINUTE, '00:00:00', [time])/15, lowPrice, highPrice
	, LAST_VALUE(price) OVER (PARTITION BY DATEDIFF(MINUTE, '00:00:00', [time])/15 ORDER BY [time] ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING) AS [price]
	, avgPrice, num
	FROM ##template_1minute) AS t 
	GROUP BY theTime
END
GO

IF OBJECT_ID('get_1day', 'P') IS NOT NULL
	DROP PROC get_1day
GO
CREATE PROC get_1day
AS
BEGIN
	SELECT MAX([date]) AS [date], '00:00:00' AS [time], MIN(lowPrice) AS lowPrice, MAX(highPrice) AS highPrice
		, MAX(price) AS price, SUM(1.0*avgPrice*num)/SUM(num) AS avgPrice, SUM(num) AS num FROM (
	SELECT [date], lowPrice, highPrice
		, LAST_VALUE(price) OVER (ORDER BY [time] ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING) AS price
		, avgPrice, num FROM ##template_15minute) AS t
	RETURN;
END
GO

IF OBJECT_ID('d_1minute', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[d_1minute](
		[code] [decimal](6, 0) NOT NULL,
		[date] [date] NOT NULL,
		[time] [time](0) NOT NULL,
		[lowPrice] [smallmoney] NULL,
		[highPrice] [smallmoney] NULL,
		[price] [smallmoney] NULL,
		[avgPrice] [smallmoney] NULL,
		[num] [int] NULL,
		[ema1] [smallmoney] NULL,
		[ema2] [smallmoney] NULL,
		[dif] [decimal](15, 6) NULL,
		[pole] [bit] NULL,
		[devia] [smallint] NULL,
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
		[code] [decimal](6, 0) NOT NULL,
		[date] [date] NOT NULL,
		[time] [time](0) NOT NULL,
		[lowPrice] [smallmoney] NULL,
		[highPrice] [smallmoney] NULL,
		[price] [smallmoney] NULL,
		[avgPrice] [smallmoney] NULL,
		[num] [int] NULL,
		[ema1] [smallmoney] NULL,
		[ema2] [smallmoney] NULL,
		[dif] [decimal](15, 6) NULL,
		[pole] [bit] NULL,
		[devia] [smallint] NULL,
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
		[code] [decimal](6, 0) NOT NULL,
		[date] [date] NOT NULL,
		[time] [time](0) NULL,
		[lowPrice] [smallmoney] NULL,
		[highPrice] [smallmoney] NULL,
		[price] [smallmoney] NULL,
		[avgPrice] [smallmoney] NULL,
		[num] [int] NULL,
		[ema1] [smallmoney] NULL,
		[ema2] [smallmoney] NULL,
		[dif] [decimal](15, 6) NULL,
		[pole] [bit] NULL,
		[devia] [smallint] NULL,
	PRIMARY KEY CLUSTERED 
	(
		[code] ASC,
		[date] ASC
	)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
	) ON [PRIMARY]
END
GO

-------------------------------------------------------
USE stock
GO

IF OBJECT_ID('tabMgr', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[tabMgr](
		[dat] [date] NULL,
		[cod] [decimal](6, 0) NULL,
		[escapeTime] [varchar](20) NULL
	) ON [PRIMARY]
END
GO
IF OBJECT_ID('tabMgrHistory', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[tabMgrHistory](
		[dat] [date] NULL,
		[cod] [decimal](6, 0) NULL,
		[escapeTime] [varchar](20) NULL
	) ON [PRIMARY]
END
GO

IF OBJECT_ID('MetaTable', 'U') IS NULL
BEGIN
	CREATE TABLE MetaTable (code DECIMAL(6, 0), type INT, lowPrice SMALLMONEY
		, highPrice SMALLMONEY, price SMALLMONEY, num INT, [date] DATE, [time] TIME(0), avgPrice SMALLMONEY
		, ema1 SMALLMONEY, ema2 SMALLMONEY, dif SMALLMONEY, prePrice SMALLMONEY
		, maxDate DATE, maxTime TIME(0), maxLastPrice SMALLMONEY, maxDif SMALLMONEY, maxDevia SMALLMONEY
		, minDate DATE, minTime TIME(0), minLastPrice SMALLMONEY, minDif SMALLMONEY, minDevia SMALLMONEY)
END 
GO










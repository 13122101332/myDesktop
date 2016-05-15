USE stock_A
GO
DECLARE t_c CURSOR LOCAL FOR
SELECT name FROM sysobjects WHERE type='u' AND name NOT LIKE 'd%'
OPEN t_c
WHILE @@FETCH_STATUS=0
BEGIN
	DECLARE @tableName VARCHAR(50)
	FETCH NEXT FROM t_c INTO @tableName
	print @tableName
	EXEC ('DROP table '+@tableName)
END
CLOSE t_c
DEALLOCATE t_c
GO
USE stock_A
GO
DELETE FROM d_1minute
DELETE FROM d_15minute
DELETE FROM d_1day
GO
USE stock
GO
DELETE FROM TempDataTable
INSERT INTO tabMgr SELECT * FROM tabMgrHistory
DELETE FROM tabMgrHistory
GO

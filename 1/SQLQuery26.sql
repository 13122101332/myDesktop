USE stock
GO
DECLARE t_c CURSOR LOCAL
FOR SELECT name FROM sysobjects WHERE type='u' AND name LIKE 's%' AND name NOT LIKE 'tabMg%'
DECLARE @tableName VARCHAR(50)
OPEN t_c
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM t_c INTO @tableName
	print 'DROP TABLE '+@tableName
	EXEC ('DROP TABLE '+@tableName)
END
CLOSE t_c
DEALLOCATE t_c
GO

DELETE FROM tabMgr
DELETE FROM tabMgrHistory
GO

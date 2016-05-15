package com.devia.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

/**
 * @author renna
 * @数据库连接
 * */
public class DB {

	private static final String usename = "sa";
	private static final String password = "saqwe123_00";
	private static final String DRIVER = "com.microsoft.sqlserver.jdbc.SQLServerDriver";
	private static final String URL ="jdbc:sqlserver://localhost:1433;databasename=stock_A;";

	
	static {
		try {
			Class.forName(DRIVER);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	public static Connection getConn() {
		try {
			Connection conn = DriverManager.getConnection(URL, usename,
					password);
			System.out.println("数据库连接成功");
			return conn;
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}
	}

	public static void close(Connection conn, Statement stmt, ResultSet rs) {
		try {
			if (rs != null)
				rs.close();
			if (stmt != null)
				stmt.close();
			if (conn != null)
				conn.close();
			System.out.println("数据库释放成功");
		} catch (SQLException e) {
			e.printStackTrace();
		}

	}

}

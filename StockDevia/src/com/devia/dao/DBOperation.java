package com.devia.dao;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.devia.model.SelectStock;

public class DBOperation {

	int pageSize = 15;// 默认一页显示15条数据

	// DB db = new DB();

	/** 查询分页数据 */
	public List<SelectStock> getDate(String date1, String date2,
			String sortdate, String code1, String code2, String sortcode,
			String devia1, String devia2, String sortdevia, String tabname,
			int cur) {
		Connection conn = DB.getConn();
		Statement stmt = null;
		ResultSet rs = null;
		List<SelectStock> list = new ArrayList<SelectStock>();
		
		String sql = "with sr as (" + " select top(select count(*) from "
				+ tabname + " )*,row_number() "
				+ " over(order by (select 0))as temp from " + tabname
				+ " where date between '" + date1 + "' AND '" + date2 + "'"
				+ " and code between " + code1 + " and " + code2
				+ " and devia between " + devia1 + " and " + devia2
				+ " order by date " + sortdate + " ,code " + sortcode
				+ ",devia " + sortdevia + ")" + " select * "
				+ " from sr where temp between " + (pageSize * (cur - 1) + 1)
				+ " and " + pageSize * (cur);
		System.out.println("SQL===" + sql);
		// String sql ="select top 10 * from d_1day";
		try {
			stmt = conn.createStatement();
			rs = stmt.executeQuery(sql);
			while (rs.next()) {
                                                                SelectStock stock = new SelectStock();
				stock.setCode(rs.getString("code"));
				stock.setDate(rs.getString("date"));
				stock.setDevia(rs.getString("devia"));
				stock.setTime(rs.getString("time"));
				stock.setPrice(rs.getString("price"));
				stock.setLowprice(rs.getString("lowPrice"));
				stock.setHighprice(rs.getString("highPrice"));
				stock.setAvgprice(rs.getString("avgPrice"));
				stock.setNum(rs.getString("num"));
				stock.setEma1(rs.getString("ema1"));
				stock.setEma2(rs.getString("ema2"));
				stock.setDif(rs.getString("dif"));

				list.add(stock);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DB.close(conn, stmt, rs);
		}
		return list;
	}

	/** 测试表是否存在 */
	public boolean existTable(String tablename) {
		Connection conn = DB.getConn();
		Statement stmt = null;
		ResultSet rs = null;
		boolean ret = false;
		String sql = "select * from " + tablename;
		try {
			stmt = conn.createStatement();
			rs = stmt.executeQuery(sql);
			while (rs.next()) {
				ret = true;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DB.close(conn, stmt, rs);
		}
		return ret;
	}

	/** 创建表 */
	public void createTable(String tablename) {
		Connection conn = DB.getConn();
		Statement stmt = null;
		ResultSet rs = null;
		String numtype = null;
		if (tablename.equals("d_1day")) {
			numtype = "int";
		}
		if (tablename.equals("d_15minute")) {
			numtype = "bigint";
		}
		if (tablename.equals("d_1minute")) {
			numtype = "bigint";
		}

		String sql = "CREATE TABLE " + tablename
				+ "(	code decimal(6, 0) NOT NULL,"
				+ "date date NOT NULL,time  time(7) NULL,"
				+ "lowPrice smallmoney  NULL,"
				+ "highPrice smallmoney  NULL, avgPrice smallmoney  NULL,"
				+ "price smallmoney NULL," + "num  " + numtype + " NULL,"
				+ "ema1 smallmoney  NULL," + "ema2 smallmoney  NULL,"
				+ "dif decimal(15, 6) NULL," + "pole smallint NULL,"
				+ "devia smallint NULL)";

		try {
			stmt = conn.createStatement();
			stmt.executeUpdate(sql);
			System.out.println("创建表成功");

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DB.close(conn, stmt, rs);
		}
	}

	/** 获取总页数 */
	public int getTotalPage(String date1, String date2, String code1,
			String code2, String devia1, String devia2, String tabname) {
		Connection conn = DB.getConn();
		Statement stmt = null;
		ResultSet rs = null;
		/*** 如果不存在，先创建表 */
		/*
		 * if (!existTable(tabname)) { createTable(tabname); }
		 */
		String sql = "select COUNT(*) from " + tabname
				+ " where date between '" + date1 + "' and '" + date2
				+ "' and code between " + code1 + " and " + code2
				+ " and devia between " + devia1 + " and " + devia2;
		int count = 0;
		int total = 0;
		try {
			stmt = conn.createStatement();
			rs = stmt.executeQuery(sql);
			while (rs.next()) {
				total = rs.getInt(1);
				System.out.println("rs.getInt(1)=" + rs.getInt(1));
				count = (int) Math.ceil((total + 1.0 - 1.0) / pageSize);

			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DB.close(conn, stmt, rs);
		}
		return count;
	}
}

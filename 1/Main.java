import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.TimeZone;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.sumup.util.TypeConver;

/**
 * @version 1.0.4
 */
public class Main {
	static final double ALPHA1 = 2.0 / 13.0;
	static final double ALPHA2 = 2.0 / 27.0;

	static Connection conn = null;
	static Connection conn_A = null;
	static Connection connBatch1 = null;
	static Connection connBatch2 = null;
	static Connection connBatch3 = null;
	// d表
	static Connection connBatch7 = null;
	static Connection connBatch8 = null;
	static Connection connBatch9 = null;
	static PreparedStatement ps = null;
	static PreparedStatement ps_A = null;
	static PreparedStatement psBatch1 = null;
	static PreparedStatement psBatch2 = null;
	static PreparedStatement psBatch3 = null;
	// d表
	static PreparedStatement psBatch7 = null;
	static PreparedStatement psBatch8 = null;
	static PreparedStatement psBatch9 = null;

	static ResultSet rs = null;

	static {
		try {
			Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	static void createAllNeededTable(Connection conn, Connection conn_A) throws SQLException {
		PreparedStatement ps = null;
		try {
			ps = conn.prepareStatement("CREATE TABLE MetaTable (code DECIMAL(6, 0), type INT, lowPrice SMALLMONEY"
					+ ", highPrice SMALLMONEY, price SMALLMONEY, num INT, date DATE, time TIME(0), avgPrice SMALLMONEY"
					+ ", ema1 SMALLMONEY, ema2 SMALLMONEY, dif SMALLMONEY, prePrice SMALLMONEY"
					+ ", maxDate DATE, maxTime TIME(0), maxLastPrice SMALLMONEY, maxDif SMALLMONEY, maxDevia SMALLMONEY"
					+ ", minDate DATE, minTime TIME(0), minLastPrice SMALLMONEY, minDif SMALLMONEY, minDevia SMALLMONEY)");
			ps.executeUpdate();
		} catch (SQLException e) {
		}
		ps.close();
		try {
			ps = conn_A.prepareStatement("CREATE TABLE ##template_1minute("
					+"[date] DATE,"
					+"[time] TIME(0),"
					+"lowPrice SMALLMONEY,"
					+"highPrice SMALLMONEY,"
					+"price SMALLMONEY,"
					+"avgPrice SMALLMONEY,"
					+"num INT"
					+")");
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
		try {
			ps = conn_A.prepareStatement("CREATE TABLE ##template_15minute("
					+"[date] DATE,"
					+"[time] TIME(0),"
					+"lowPrice SMALLMONEY,"
					+"highPrice SMALLMONEY,"
					+"price SMALLMONEY,"
					+"avgPrice SMALLMONEY,"
					+"num INT"
					+")");
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
		// d表
	}

	public static void bg() throws SQLException {
		conn = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock;", "sa", "saqwe123_00");
		conn_A = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		ps = null;
	}

	public static void bgTable() throws SQLException {
		connBatch1 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch2 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch3 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch7 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch8 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch9 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch1.setAutoCommit(false);
		connBatch2.setAutoCommit(false);
		connBatch3.setAutoCommit(false);
		connBatch7.setAutoCommit(false);
		connBatch8.setAutoCommit(false);
		connBatch9.setAutoCommit(false);
	}

	public static void edTable() throws SQLException {
		psBatch1.executeBatch();
		psBatch2.executeBatch();
		psBatch3.executeBatch();
		psBatch7.executeBatch();
		psBatch8.executeBatch();
		psBatch9.executeBatch();
		connBatch1.commit();
		connBatch2.commit();
		connBatch3.commit();
		connBatch7.commit();
		connBatch8.commit();
		connBatch9.commit();
		connBatch1.setAutoCommit(true);
		connBatch2.setAutoCommit(true);
		connBatch3.setAutoCommit(true);
		connBatch7.setAutoCommit(true);
		connBatch8.setAutoCommit(true);
		connBatch9.setAutoCommit(true);
		connBatch1.close();
		connBatch2.close();
		connBatch3.close();
		connBatch7.close();
		connBatch8.close();
		connBatch9.close();
	}

	static boolean testAndDelete(Connection conn, String code, String bgDate, String edDate) throws SQLException {
		code = TypeConver.getCodeNum(code);
		PreparedStatement ps = null;

		String fullCode = TypeConver.getFullCode(code);
		String tableName1 = fullCode + "_1MINUTE";
		String tableName2 = fullCode + "_15MINUTE";
		String tableName3 = fullCode + "_1DAY";
		try {
			ps = conn.prepareStatement("DELETE FROM " + tableName1 + " WHERE [date]>=? AND [date]<=?");
			ps.setString(1, bgDate);
			ps.setString(2, edDate);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
			if (ps.isClosed() == false)
				ps.close();
		}
		try {
			ps = conn.prepareStatement("DELETE FROM " + tableName2 + " WHERE [date]>=? AND [date]<=?");
			ps.setString(1, bgDate);
			ps.setString(2, edDate);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
			if (ps.isClosed() == false)
				ps.close();
		}
		try {
			ps = conn.prepareStatement("DELETE FROM " + tableName3 + " WHERE [date]>=? AND [date]<=?");
			ps.setString(1, bgDate);
			ps.setString(2, edDate);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
			if (ps.isClosed() == false)
				ps.close();
		}
		try {
			ps = conn.prepareStatement("DELETE FROM d_1minute WHERE code=? AND [date]>=? AND [date]<=?");
			ps.setString(1, code);
			ps.setString(2, bgDate);
			ps.setString(3, edDate);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
			if (ps.isClosed() == false)
				ps.close();
		}
		try {
			ps = conn.prepareStatement("DELETE FROM d_15minute WHERE code=? AND [date]>=? AND [date]<=?");
			ps.setString(1, code);
			ps.setString(2, bgDate);
			ps.setString(3, edDate);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
			if (ps.isClosed() == false)
				ps.close();
		}
		try {
			ps = conn.prepareStatement("DELETE FROM d_1day WHERE code=? AND [date]>=? AND [date]<=?");
			ps.setString(1, code);
			ps.setString(2, bgDate);
			ps.setString(3, edDate);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
			if (ps.isClosed() == false)
				ps.close();
		}
		// DELETE FROM tabMgrHistory WHERE cod=? AND dat>=? AND dat<=?
		try {
			ps = conn.prepareStatement("DELETE FROM tabMgrHistory WHERE cod=? AND dat>=? AND dat<=?");
			ps.setString(1, code);
			ps.setString(2, bgDate);
			ps.setString(3, edDate);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
			if (ps.isClosed() == false)
				ps.close();
		}

		return true;
	}

	static void setPole(Saver pSaver, Saver saver) {
		if (pSaver.prePrice > pSaver.lastPrice && pSaver.lastPrice < saver.lastPrice) {
			pSaver.pole = 1;
		} else if (pSaver.prePrice < pSaver.lastPrice && pSaver.lastPrice > saver.lastPrice) {
			pSaver.pole = (-1);
		} else {
			pSaver.pole = (0);
		}
	}

	static void setDevia(Saver pSaver, Saver pMax, Saver pMin) {
		if (pSaver.pole == 1) {
			if (pMin.lastPrice > pSaver.lastPrice && pMin.dif < pSaver.dif) {
				pSaver.devia = (pMin.devia + 1);
			} else {
				pSaver.devia = (0);
			}
		} else if (pSaver.pole == -1) {
			if (pMax.lastPrice < pSaver.lastPrice && pMax.dif > pSaver.dif) {
				pSaver.devia = (pMax.devia - 1);
			} else {
				pSaver.devia = (0);
			}
		} else {
			pSaver.devia = (0);
		}
	}

	static void updateSaver(Saver pSaver, Saver pMax, Saver pMin, Saver saver) {
		if(pSaver.pole == 1) {
			pMin.cloneExceptPrepri(pSaver);
		} else if(pSaver.pole == -1) {
			pMax.cloneExceptPrepri(pSaver);
		}
		pSaver.prePrice = pSaver.lastPrice;
		saver.exists = true;
		pSaver.cloneExceptPrepri(saver);
	}

	static void setCens(Saver pSaver, Saver saver) {
		if (pSaver.exists == false) {
			saver.ema1 = (saver.lastPrice);
			saver.ema2 = (saver.lastPrice);
		} else {
			saver.ema1 = (ALPHA1 * (saver.lastPrice) + (1 - ALPHA1) * pSaver.ema1);
			saver.ema2 = (ALPHA2 * (saver.lastPrice) + (1 - ALPHA2) * pSaver.ema2);
		}
		saver.dif = (saver.ema1 - saver.ema2);
	}

	public static void main(String[] args) throws Exception {
		System.out.println(System.currentTimeMillis());
		long logTime = System.currentTimeMillis();
		File file = new File("C:\\Users\\QiLIn\\Desktop\\out" + logTime + ".txt");
		if (file.exists())
			file.delete();
		file.createNewFile();
		FileOutputStream fos = new FileOutputStream(file);
		OutputStreamWriter osw = new OutputStreamWriter(fos);
		BufferedWriter bw = new BufferedWriter(osw);

		bg();

		createAllNeededTable(conn, conn_A);

		ps = conn.prepareStatement("SELECT distinct cod from tabMgr order by cod");
		rs = ps.executeQuery();
		List<String> codes = new ArrayList<>();
		while (rs.next()) {
			String code = rs.getString(1);
			codes.add(code);
		}
		rs.close();
		ps.close();
		LinkedList<String> dates = new LinkedList<String>();

		DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		df.setTimeZone(TimeZone.getTimeZone("GMT+0"));

		Saver pSaver1minute = new Saver(); // Useful
		Saver pSaver15minute = new Saver(); // Useful
		Saver pSaver1day = new Saver(); // Useful
		Saver pMax1minute = new Saver(); // Useful
		Saver pMax15minute = new Saver(); // Useful
		Saver pMax1day = new Saver(); // Useful
		Saver pMin1minute = new Saver(); // Useful
		Saver pMin15minute = new Saver(); // Useful
		Saver pMin1day = new Saver(); // Useful
		Saver saver1minute = new Saver();
		Saver saver15minute = new Saver();
		Saver saver1day = new Saver();
		ExecutorService executor = Executors.newFixedThreadPool(2);
		Callable<Integer> readTask = new Callable<Integer>() {
			@Override
			public Integer call() throws Exception {
				return System.in.read();
			}
		};
		String bgDate, edDate;
		int codeCount = 1;
		int codeCountTempIndex = 0;
		for (String code : codes) {
			if(codeCountTempIndex >= 10)
				break;
			codeCountTempIndex++;
			///////////////////////////////////////////////////////////
			boolean isRead = false;
			java.util.concurrent.Future<Integer> future = executor.submit(readTask);
			try {
				future.get(2000, TimeUnit.MILLISECONDS);
				isRead = true;
			} catch (TimeoutException e) {
			}
			if (isRead) {
				bw.write("used time: " + (System.currentTimeMillis() - logTime));
				bw.flush();
				fos.close();
				System.exit(0);
			}
			///////////////////////////////////////////////////////////

			dates.clear();

			ps = conn.prepareStatement("SELECT dat FROM tabMgr WHERE cod=? and dat>='2013-07-01' ORDER BY dat ASC");
			ps.setString(1, code);

			rs = ps.executeQuery();
			while (rs.next()) {
				String date = rs.getString(1);
				dates.add(date);
			}
			rs.close();
			ps.close();

			bgDate = dates.getFirst();
			edDate = dates.getLast();

			if (testAndDelete(conn_A, code, bgDate, edDate) == false) {
				continue;
			}

			bw.write("code: " + code + " bg " + codeCount + " begin date: " + bgDate + " end date: " + edDate + "\r\n");
			bw.flush();

			bgTable();

			String fullCode = TypeConver.getFullCode(code);
			String tableName1 = fullCode + "_1minute";
			String tableName2 = fullCode + "_15minute";
			String tableName3 = fullCode + "_1day";
			////////////////////////////////////////////////////////////////////////
			createColTable(tableName1, tableName2, tableName3);
			////////////////////////////////////////////////////////////////////////

			openBatch(tableName1, tableName2, tableName3);

			// TODO 在这里初始化
			ps = conn.prepareStatement(
					"SELECT lowPrice, highPrice, price, num, date, time, avgPrice, ema1, ema2, dif, prePrice"
							+ ", maxDate, maxTime, maxLastPrice, maxDif, maxDevia, minDate, minTime, minLastPrice, minDif, minDevia"
							+ " FROM MetaTable WHERE code=? AND type=1");
			ps.setString(1, code);
			rs = ps.executeQuery();
			if (rs.next()) {
				pSaver1minute.initPSaver(rs);
				pMax1minute.initPMax(rs);
				pMin1minute.initPMin(rs);
			} else {
				pSaver1minute.initPSaver();
				pMax1minute.initPMax();
				pMin1minute.initPMin();
			}
			rs.close();
			ps.close();
			ps = conn.prepareStatement(
					"SELECT lowPrice, highPrice, price, num, date, time, avgPrice, ema1, ema2, dif, prePrice"
							+ ", maxDate, maxTime, maxLastPrice, maxDif, maxDevia, minDate, minTime, minLastPrice, minDif, minDevia"
							+ " FROM MetaTable WHERE code=? AND type=2");
			ps.setString(1, code);
			rs = ps.executeQuery();
			if (rs.next()) {
				pSaver15minute.initPSaver(rs);
				pMax15minute.initPMax(rs);
				pMin15minute.initPMin(rs);
			} else {
				pSaver15minute.initPSaver();
				pMax15minute.initPMax();
				pMin15minute.initPMin();
			}
			rs.close();
			ps.close();
			ps = conn.prepareStatement(
					"SELECT lowPrice, highPrice, price, num, date, time, avgPrice, ema1, ema2, dif, prePrice"
							+ ", maxDate, maxTime, maxLastPrice, maxDif, maxDevia, minDate, minTime, minLastPrice, minDif, minDevia"
							+ " FROM MetaTable WHERE code=? AND type=3");
			ps.setString(1, code);
			rs = ps.executeQuery();
			if (rs.next()) {
				pSaver1day.initPSaver(rs);
				pMax1day.initPMax(rs);
				pMin1day.initPMin(rs);
			} else {
				pSaver1day.initPSaver();
				pMax1day.initPMax();
				pMin1day.initPMin();
			}
			rs.close();
			ps.close();
			while (dates.isEmpty() == false) {
				String date = dates.removeFirst();
				String dateNum = TypeConver.getDateNum(date);
				String tableName = "tab_" + dateNum + fullCode;
				ps_A = conn_A.prepareStatement("DELETE FROM ##template_1minute");
				ps_A.executeUpdate();
				ps_A.close();
				ps_A = conn_A.prepareStatement("DELETE FROM ##template_15minute");
				ps_A.executeUpdate();
				ps_A.close();
				ps_A = conn_A.prepareStatement("EXEC to_temp_table_1minute " + tableName+", '"+date+"'");
				ps_A.executeUpdate();
				ps_A.close();
				ps_A = conn_A.prepareStatement("EXEC to_temp_table_15minute ");
				ps_A.executeUpdate();
				ps_A.close();
				boolean isContinue = false;
				try {
					ps = conn.prepareStatement("SELECT TOP 1 * FROM " + tableName);
					ps.executeQuery();
				} catch (SQLException e) {
					isContinue = true;
				}
				if (ps.isClosed() == false) {
					ps.close();
				}
				if (isContinue) {
					continue;
				}

				//////////////////// START HERE ///////////////////////////

//				String fullDate = TypeConver.getFullDate(date);

				ps_A = conn_A.prepareStatement(
						"SELECT [date], [time], lowPrice, highPrice, price, avgPrice, num FROM ##template_1minute ORDER BY [time] ASC");
				rs = ps_A.executeQuery();
				while (rs.next()) {
					saver1minute.date = rs.getString("date");
					saver1minute.time = rs.getString("time").substring(0, 5).toCharArray();
					saver1minute.lowPrice = rs.getDouble("lowPrice");
					saver1minute.highPrice = rs.getDouble("highPrice");
					saver1minute.lastPrice = rs.getDouble("price");
					saver1minute.sum = rs.getInt("num");
					saver1minute.avgSummer = rs.getDouble("avgPrice") * saver1minute.sum;
					setCens(pSaver1minute, saver1minute);
					setPole(pSaver1minute, saver1minute);
					setDevia(pSaver1minute, pMax1minute, pMin1minute);

					addBatch(psBatch1, psBatch7, pSaver1minute, code, false);

					updateSaver(pSaver1minute, pMax1minute, pMin1minute, saver1minute);
				}
				rs.close(); ps_A.close();

				ps_A = conn_A.prepareStatement(
						"SELECT [date], [time], lowPrice, highPrice, price, avgPrice, num FROM ##template_15minute");
				rs = ps_A.executeQuery();
				while (rs.next()) {
					saver15minute.date = rs.getString("date");
					saver15minute.time = rs.getString("time").substring(0, 5).toCharArray();
					saver15minute.lowPrice = rs.getDouble("lowPrice");
					saver15minute.highPrice = rs.getDouble("highPrice");
					saver15minute.lastPrice = rs.getDouble("price");
					saver15minute.sum = rs.getInt("num");
					saver15minute.avgSummer = rs.getDouble("avgPrice") * saver15minute.sum;
					setCens(pSaver15minute, saver15minute);
					setPole(pSaver15minute, saver15minute);
					setDevia(pSaver15minute, pMax15minute, pMin15minute);

					addBatch(psBatch2, psBatch8, pSaver15minute, code, false);

					updateSaver(pSaver15minute, pMax15minute, pMin15minute, saver15minute);
				}
				rs.close(); ps_A.close();

				ps_A = conn_A.prepareStatement("EXEC get_1day");
				rs = ps_A.executeQuery();
				while(rs.next()) {
					saver1day.date = rs.getString("date");
					saver1day.time = rs.getString("time").substring(0, 5).toCharArray();
					saver1day.lowPrice = rs.getDouble("lowPrice");
					saver1day.highPrice = rs.getDouble("highPrice");
					saver1day.lastPrice = rs.getDouble("price");
					saver1day.sum = rs.getInt("num");
					saver1day.avgSummer = rs.getDouble("avgPrice") * saver1day.sum;
					setCens(pSaver1day, saver1day);
					setPole(pSaver1day, saver1day);
					setDevia(pSaver1day, pMax1day, pMin1day);

					addBatch(psBatch3, psBatch9, pSaver1day, code, true);

					updateSaver(pSaver1day, pMax1day, pMin1day, saver1day);
				}
				rs.close(); ps_A.close();
			}

			edTable();

			ps = conn.prepareStatement(
					"INSERT INTO tabMgrHistory SELECT * FROM tabMgr WHERE cod=? AND dat>=? AND dat<=?");
			ps.setString(1, code);
			ps.setString(2, bgDate);
			ps.setString(3, edDate);
			ps.executeUpdate();
			ps.close();
			ps = conn.prepareStatement("DELETE FROM tabMgr WHERE cod=? AND dat>=? AND dat<=?");
			ps.setString(1, code);
			ps.setString(2, bgDate);
			ps.setString(3, edDate);
			ps.executeUpdate();
			ps.close();

			updateMetaTable(conn, code, pSaver1minute, pMax1minute, pMin1minute, 1);
			updateMetaTable(conn, code, pSaver15minute, pMax15minute, pMin15minute, 2);
			updateMetaTable(conn, code, pSaver1day, pMax1day, pMin1day, 3);

			bw.write("code: " + code + " " + (codeCount++) + "\r\n");
			bw.flush();
		}

		conn.close();

		bw.write("used time: " + (System.currentTimeMillis() - logTime));
		bw.flush();
		fos.close();

		System.out.println(System.currentTimeMillis());
	}

	static void updateMetaTable(Connection conn, String code, Saver pSaver, Saver pMax, Saver pMin, int type) throws SQLException {
		PreparedStatement ps = conn.prepareStatement("DELETE FROM MetaTable WHERE code=? AND type=?");
		ps.setString(1, code);
		ps.setInt(2, type);
		ps.executeUpdate(); ps.close();
		String pTime = String.valueOf(pSaver.time);
		String maxTime = String.valueOf(pMax.time);
		String minTime = String.valueOf(pMin.time);
		if(type == 3) {
			pTime = "00:00:00";
			maxTime = "00:00:00";
			minTime = "00:00:00";
		}
		ps = conn.prepareStatement("INSERT INTO MetaTable VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		ps.setString(1, code); ps.setInt(2, type); ps.setDouble(3, pSaver.lowPrice); ps.setDouble(4, pSaver.highPrice);
		ps.setDouble(5, pSaver.lastPrice); ps.setInt(6, pSaver.sum); ps.setString(7, pSaver.date); ps.setString(8, pTime);
		ps.setDouble(9, pSaver.avgSummer/pSaver.sum); ps.setDouble(10, pSaver.ema1); ps.setDouble(11, pSaver.ema2);
		ps.setDouble(12, pSaver.dif); ps.setDouble(13, pSaver.prePrice);
		ps.setString(14, pMax.date); ps.setString(15, maxTime); ps.setDouble(16, pMax.lastPrice); ps.setDouble(17, pMax.dif); ps.setDouble(18, pMax.devia);
		ps.setString(19, pMin.date); ps.setString(20, minTime); ps.setDouble(21, pMin.lastPrice); ps.setDouble(22, pMin.dif); ps.setDouble(23, pMin.devia);
	}

	private static void openBatch(String tableName1, String tableName2, String tableName3) throws SQLException {
		psBatch1 = connBatch1
				.prepareStatement("INSERT INTO " + tableName1 + " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch2 = connBatch2
				.prepareStatement("INSERT INTO " + tableName2 + " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch3 = connBatch3
				.prepareStatement("INSERT INTO " + tableName3 + " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch7 = connBatch7.prepareStatement("INSERT INTO d_1minute VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch8 = connBatch8.prepareStatement("INSERT INTO d_15minute VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch9 = connBatch9.prepareStatement("INSERT INTO d_1day VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	}

	private static void createColTable(String tableName1, String tableName2, String tableName3) {
		PreparedStatement ps = null;
		try {
			String sql = "CREATE TABLE " + tableName1
					+ " ([date] DATE, [time] TIME(0), lowPrice SMALLMONEY, highPrice SMALLMONEY, price SMALLMONEY, avgPrice SMALLMONEY, num INT, ema1 SMALLMONEY, ema2 SMALLMONEY, dif DECIMAL(15, 6)"
					+ ", pole INT, devia INT, PRIMARY KEY([date] ASC, [time] ASC))";
			ps = conn_A.prepareStatement(sql);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
		try {
			String sql = "CREATE TABLE " + tableName2
					+ " ([date] DATE, [time] TIME(0), lowPrice SMALLMONEY, highPrice SMALLMONEY, price SMALLMONEY, avgPrice SMALLMONEY, num INT, ema1 SMALLMONEY, ema2 SMALLMONEY, dif DECIMAL(15, 6)"
					+ ", pole INT, devia INT, PRIMARY KEY([date] ASC, [time] ASC))";
			ps = conn_A.prepareStatement(sql);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
		try {
			String sql = "CREATE TABLE " + tableName3
					+ " ([date] DATE, lowPrice SMALLMONEY, highPrice SMALLMONEY, price SMALLMONEY, avgPrice SMALLMONEY, num INT, ema1 SMALLMONEY, ema2 SMALLMONEY, dif DECIMAL(15, 6)"
					+ ", pole INT, devia INT, PRIMARY KEY([date] ASC))";
			ps = conn_A.prepareStatement(sql);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
	}

	static void addBatch(PreparedStatement psSaver, PreparedStatement psD, Saver pSaver, String code, boolean isDay)
			throws SQLException {
		if(pSaver.date==null || pSaver.date.equals(""))
			return;
		if (isDay) {
			addBatch(psSaver, pSaver.date, pSaver.lowPrice, pSaver.highPrice, pSaver.lastPrice,
					pSaver.avgSummer / pSaver.sum, pSaver.sum, pSaver.ema1, pSaver.ema2, pSaver.dif, pSaver.pole,
					pSaver.devia);
		} else {
			addBatch(psSaver, pSaver.date, String.valueOf(pSaver.time), pSaver.lowPrice, pSaver.highPrice,
					pSaver.lastPrice, pSaver.avgSummer / pSaver.sum, pSaver.sum, pSaver.ema1, pSaver.ema2, pSaver.dif,
					pSaver.pole, pSaver.devia);
		}
		if (pSaver.devia != 0) {
			if(isDay) {
				addBatch(psD, code, pSaver.date, "00:00:00", pSaver.lowPrice, pSaver.highPrice,
						pSaver.lastPrice, pSaver.avgSummer / pSaver.sum, pSaver.sum, pSaver.ema1, pSaver.ema2, pSaver.dif,
						pSaver.pole, pSaver.devia);
			} else {
				addBatch(psD, code, pSaver.date, String.valueOf(pSaver.time), pSaver.lowPrice, pSaver.highPrice,
						pSaver.lastPrice, pSaver.avgSummer / pSaver.sum, pSaver.sum, pSaver.ema1, pSaver.ema2, pSaver.dif,
						pSaver.pole, pSaver.devia);
			}
		}
	}

	// [date] DATE, [time] TIME(0), price SMALLMONEY, num INT, dif DECIMAL(15,
	// 6), ema1 SMALLMONEY, ema2 SMALLMONEY, highPrice SMALLMONEY, lowPrice
	// SMALLMONEY, avgPrice SMALLMONEY
	public static void addBatch(PreparedStatement ps, String date, String time, double lowPrice, double highPrice,
			double price, double avgPrice, int num, double ema1, double ema2, double dif, int pole, int devia)
					throws SQLException {
		price = TypeConver.restrDouble(price);
		dif = TypeConver.restrDouble(dif);
		ema1 = TypeConver.restrDouble(ema1);
		ema2 = TypeConver.restrDouble(ema2);
		highPrice = TypeConver.restrDouble(highPrice);
		lowPrice = TypeConver.restrDouble(lowPrice);
		avgPrice = TypeConver.restrDouble(avgPrice);

		ps.setString(1, date);
		ps.setString(2, time);
		ps.setDouble(3, lowPrice);
		ps.setDouble(4, highPrice);
		ps.setDouble(5, price);
		ps.setDouble(6, avgPrice);
		ps.setInt(7, num);
		ps.setDouble(8, ema1);
		ps.setDouble(9, ema2);
		ps.setDouble(10, dif);
		ps.setInt(11, pole);
		ps.setInt(12, devia);

		ps.addBatch();
	}

	public static void addBatch(PreparedStatement ps, String date, double lowPrice, double highPrice, double price,
			double avgPrice, int num, double ema1, double ema2, double dif, int pole, int devia) throws SQLException {
		price = TypeConver.restrDouble(price);
		dif = TypeConver.restrDouble(dif);
		ema1 = TypeConver.restrDouble(ema1);
		ema2 = TypeConver.restrDouble(ema2);
		highPrice = TypeConver.restrDouble(highPrice);
		lowPrice = TypeConver.restrDouble(lowPrice);
		avgPrice = TypeConver.restrDouble(avgPrice);

		ps.setString(1, date);
		ps.setDouble(2, lowPrice);
		ps.setDouble(3, highPrice);
		ps.setDouble(4, price);
		ps.setDouble(5, avgPrice);
		ps.setInt(6, num);
		ps.setDouble(7, ema1);
		ps.setDouble(8, ema2);
		ps.setDouble(9, dif);
		ps.setInt(10, pole);
		ps.setInt(11, devia);

		ps.addBatch();
	}

	// code DECIMAL(6, 0), [date] DATE, [time] TIME(0), pole SMALLINT, devia
	// SMALLINT, price SMALLMONEY, num INT, dif DECIMAL(15, 6), ema1 SMALLMONEY,
	// ema2 SMALLMONEY, highPrice SMALLMONEY, lowPrice SMALLMONEY, avgPrice
	// SMALLMONEY
	public static void addBatch(PreparedStatement ps, String code, String date, String time, double lowPrice,
			double highPrice, double price, double avgPrice, int num, double ema1, double ema2, double dif, int pole,
			int devia) throws SQLException {
		if (pole == 0)
			return;
		if (time == null || time.equals(""))
			time = "00:00:00";
		code = TypeConver.getCodeNum(code);
		price = TypeConver.restrDouble(price);
		dif = TypeConver.restrDouble(dif);
		ema1 = TypeConver.restrDouble(ema1);
		ema2 = TypeConver.restrDouble(ema2);
		highPrice = TypeConver.restrDouble(highPrice);
		lowPrice = TypeConver.restrDouble(lowPrice);
		avgPrice = TypeConver.restrDouble(avgPrice);

		ps.setString(1, code);
		ps.setString(2, date);
		ps.setString(3, time);
		ps.setDouble(4, lowPrice);
		ps.setDouble(5, highPrice);
		ps.setDouble(6, price);
		ps.setDouble(7, avgPrice);
		ps.setInt(8, num);
		ps.setDouble(9, ema1);
		ps.setDouble(10, ema2);
		ps.setDouble(11, dif);
		ps.setInt(12, pole);
		ps.setInt(13, devia);
		//
		ps.addBatch();
	}
}

class Saver {
	public String date = "";
	public char[] time = new char[5];
	public double lastPrice;
	public double lowPrice;
	public double highPrice;
	public double avgSummer;
	public int sum;
	public double ema1;
	public double ema2;
	public double dif;

	public double prePrice;

	public boolean exists;
	public int pole; // -1 max 1 min
	public int devia;

	public void cloneExceptPrepri(Saver saver) {
		date = saver.date;
		time = Arrays.copyOf(saver.time, 5);
		lastPrice = saver.lastPrice;
		lowPrice = saver.lowPrice;
		highPrice = saver.highPrice;
		avgSummer = saver.avgSummer;
		sum = saver.sum;
		ema1 = saver.ema1;
		ema2 = saver.ema2;
		dif = saver.dif;

		exists = saver.exists;
		pole = saver.pole;
		devia = saver.devia;
	}

	public void initPSaver() {
		prePrice = -1;
		exists = false;
	}

	public void initPSaver(ResultSet rs) throws SQLException {
		exists = true;

		lowPrice = rs.getDouble("lowPrice");
		highPrice = rs.getDouble("highPrice");
		lastPrice = rs.getDouble("price");
		sum = rs.getInt("num");
		date = rs.getString("date");
		time = rs.getString("time").substring(0, 5).toCharArray();
		double avgPrice = rs.getDouble("avgPrice");
		avgSummer = sum * avgPrice;
		ema1 = rs.getDouble("ema1");
		ema2 = rs.getDouble("ema2");
		dif = rs.getDouble("dif");

		prePrice = rs.getDouble("price");
	}

	public void initPMin() {
		lastPrice = -1;
	}

	public void initPMin(ResultSet rs) throws SQLException {
		this.date = rs.getString("minDate");
		this.time = rs.getString("minTime").substring(0, 5).toCharArray();
		lastPrice = rs.getDouble("minLastPrice");
		dif = rs.getDouble("minDif");
		devia = rs.getInt("minDevia");
	}

	public void initPMax() {
		lastPrice = 99999999;
	}

	public void initPMax(ResultSet rs) throws SQLException {
		this.date = rs.getString("maxDate");
		this.time = rs.getString("maxTime").substring(0, 5).toCharArray();
		lastPrice = rs.getDouble("maxLastPrice");

		dif = rs.getDouble("maxDif");
		devia = rs.getInt("maxDevia");
	}

	public void accum(Saver src) {
		lastPrice = src.lastPrice;
		if (highPrice < src.highPrice)
			highPrice = src.highPrice;
		if (lowPrice > src.lowPrice)
			lowPrice = src.lowPrice;
		avgSummer += src.avgSummer;
		sum += src.sum;
	}

	public void initSaver() {
		lowPrice = 99999999; // OK
		highPrice = 0; // OK
		lastPrice = -1; // OK
		avgSummer = 0; // OK
		sum = 0; // OK

		exists = false; // OK
	}

	public void accum(Tab src) {
		lastPrice = src.currentPrice;
		if (highPrice < src.currentPrice) {
			highPrice = src.currentPrice;
		}
		if (lowPrice > src.currentPrice) {
			lowPrice = src.currentPrice;
		}
		avgSummer += src.num * src.currentPrice;
		sum += src.num;
	}
}

class Tab {
	public int num = 0;
	public double currentPrice = 0;
	public char[] time = new char[5];
}

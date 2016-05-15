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
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.sumup.util.TypeConver;

class Task extends TimerTask {

	@Override
	public void run() {
		System.exit(-1);
	}
	
}

/**
 * 没有插入，删除，更新
 * 没有初始化中的查询
 * 有了归结表的插入
 * 有了PD和D表的插入
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
	static Connection connBatch4 = null;
	static Connection connBatch5 = null;
	static Connection connBatch6 = null;
	// d表
	static Connection connBatch7 = null;
	static Connection connBatch8 = null;
	static Connection connBatch9 = null;
	static PreparedStatement ps = null;
	static PreparedStatement ps_A = null;
	static PreparedStatement psBatch1 = null;
	static PreparedStatement psBatch2 = null;
	static PreparedStatement psBatch3 = null;
	static PreparedStatement psBatch4 = null;
	static PreparedStatement psBatch5 = null;
	static PreparedStatement psBatch6 = null;
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

	public static void bg() throws SQLException {
		conn = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock;", "sa", "saqwe123_00");
		conn_A = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		ps = null;
	}

	public static void bgTable() throws SQLException {
		connBatch1 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock;", "sa",
				"saqwe123_00");
		connBatch2 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock;", "sa",
				"saqwe123_00");
		connBatch3 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock;", "sa",
				"saqwe123_00");
		connBatch4 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch5 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
				"saqwe123_00");
		connBatch6 = DriverManager.getConnection("jdbc:sqlserver://127.0.0.1:1433;databaseName=stock_A;", "sa",
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
		connBatch4.setAutoCommit(false);
		connBatch5.setAutoCommit(false);
		connBatch6.setAutoCommit(false);
		connBatch7.setAutoCommit(false);
		connBatch8.setAutoCommit(false);
		connBatch9.setAutoCommit(false);
	}

	public static void edTable() throws SQLException {
		psBatch1.executeBatch();
		psBatch2.executeBatch();
		psBatch3.executeBatch();
		psBatch4.executeBatch();
		psBatch5.executeBatch();
		psBatch6.executeBatch();
		psBatch7.executeBatch();
		psBatch8.executeBatch();
		psBatch9.executeBatch();
		connBatch1.commit();
		connBatch2.commit();
		connBatch3.commit();
		connBatch4.commit();
		connBatch5.commit();
		connBatch6.commit();
		connBatch7.commit();
		connBatch8.commit();
		connBatch9.commit();
		connBatch1.setAutoCommit(true);
		connBatch2.setAutoCommit(true);
		connBatch3.setAutoCommit(true);
		connBatch4.setAutoCommit(true);
		connBatch5.setAutoCommit(true);
		connBatch6.setAutoCommit(true);
		connBatch7.setAutoCommit(true);
		connBatch8.setAutoCommit(true);
		connBatch9.setAutoCommit(true);
		connBatch1.close();
		connBatch2.close();
		connBatch3.close();
		connBatch4.close();
		connBatch5.close();
		connBatch6.close();
		connBatch7.close();
		connBatch8.close();
		connBatch9.close();
	}

	static boolean testAndDelete(String code, String bgDate, String edDate) throws SQLException {
		code = TypeConver.getCodeNum(code);

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
			ps_A = conn_A.prepareStatement("DELETE FROM pd_1minute WHERE code=? AND [date]>=? AND [date]<=?");
			ps_A.setString(1, code);
			ps_A.setString(2, bgDate);
			ps_A.setString(3, edDate);
			ps_A.executeUpdate();
			ps_A.close();
		} catch (SQLException e) {
			if (ps_A.isClosed() == false)
				ps_A.close();
		}
		try {
			ps_A = conn_A.prepareStatement("DELETE FROM pd_15minute WHERE code=? AND [date]>=? AND [date]<=?");
			ps_A.setString(1, code);
			ps_A.setString(2, bgDate);
			ps_A.setString(3, edDate);
			ps_A.executeUpdate();
			ps_A.close();
		} catch (SQLException e) {
			if (ps_A.isClosed() == false)
				ps_A.close();
		}
		try {
			ps_A = conn_A.prepareStatement("DELETE FROM pd_1day WHERE code=? AND [date]>=? AND [date]<=?");
			ps_A.setString(1, code);
			ps_A.setString(2, bgDate);
			ps_A.setString(3, edDate);
			ps_A.executeUpdate();
			ps_A.close();
		} catch (SQLException e) {
			if (ps_A.isClosed() == false)
				ps_A.close();
		}
		try {
			ps_A = conn_A.prepareStatement("DELETE FROM d_1minute WHERE code=? AND [date]>=? AND [date]<=?");
			ps_A.setString(1, code);
			ps_A.setString(2, bgDate);
			ps_A.setString(3, edDate);
			ps_A.executeUpdate();
			ps_A.close();
		} catch (SQLException e) {
			if (ps_A.isClosed() == false)
				ps_A.close();
		}
		try {
			ps_A = conn_A.prepareStatement("DELETE FROM d_15minute WHERE code=? AND [date]>=? AND [date]<=?");
			ps_A.setString(1, code);
			ps_A.setString(2, bgDate);
			ps_A.setString(3, edDate);
			ps_A.executeUpdate();
			ps_A.close();
		} catch (SQLException e) {
			if (ps_A.isClosed() == false)
				ps_A.close();
		}
		try {
			ps_A = conn_A.prepareStatement("DELETE FROM d_1day WHERE code=? AND [date]>=? AND [date]<=?");
			ps_A.setString(1, code);
			ps_A.setString(2, bgDate);
			ps_A.setString(3, edDate);
			ps_A.executeUpdate();
			ps_A.close();
		} catch (SQLException e) {
			if (ps_A.isClosed() == false)
				ps_A.close();
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

	static void judgePole(Saver pSaver, Saver saver) {
		if (pSaver.prePrice > pSaver.lastPrice && pSaver.lastPrice < saver.lastPrice) {
			pSaver.pole = 1;
		} else if (pSaver.prePrice < pSaver.lastPrice && pSaver.lastPrice > saver.lastPrice) {
			pSaver.pole = (-1);
		} else {
			pSaver.pole = (0);
		}
	}

	static void judgeDevia(Saver pSaver, Saver pMin, Saver pMax, String code, PreparedStatement ps,
			PreparedStatement psD) throws Exception {
		if (pSaver.pole == 1) {
			if (pMin.lastPrice > pSaver.lastPrice && pMin.dif < pSaver.dif) {
				pSaver.devia = (pMin.devia + 1);
			} else {
				pSaver.devia = (0);
			}
			pMin.cloneExceptPrepri(pSaver);

			String time = String.valueOf(pMin.time);
			addBatch(ps, code, pSaver.date, time, pMin.lowPrice, pMin.highPrice, pMin.lastPrice,
					pMin.avgSummer / pMin.sum, pMin.sum, pMin.ema1, pMin.ema2, pMin.dif, pMin.pole, pMin.devia);
			if (pMin.devia != 0) {
				addBatch(psD, code, pSaver.date, time, pMin.lowPrice, pMin.highPrice, pMin.lastPrice,
						pMin.avgSummer / pMin.sum, pMin.sum, pMin.ema1, pMin.ema2, pMin.dif, pMin.pole, pMin.devia);
			}
		}
		if (pSaver.pole == -1) {
			if (pMax.lastPrice < pSaver.lastPrice && pMax.dif > pSaver.dif) {
				pSaver.devia = (pMax.devia - 1);
			} else {
				pSaver.devia = (0);
			}
			pMax.cloneExceptPrepri(pSaver);

			String time = String.valueOf(pMax.time);
			addBatch(ps, code, pSaver.date, time, pMax.lowPrice, pMax.highPrice, pMax.lastPrice,
					pMax.avgSummer / pMax.sum, pMax.sum, pMax.ema1, pMax.ema2, pMax.dif, pMax.pole, pMax.devia);
			if (pMax.devia != 0) {
				addBatch(psD, code, pSaver.date, time, pMax.lowPrice, pMax.highPrice, pMax.lastPrice,
						pMax.avgSummer / pMax.sum, pMax.sum, pMax.ema1, pMax.ema2, pMax.dif, pMax.pole, pMax.devia);
			}
		}
	}

	static void setCens(Saver pSaver, Saver saver) {
		if (pSaver.exists == false) {
			saver.ema1 = (saver.lastPrice);
			saver.ema2 = (saver.lastPrice);
			saver.dif = (saver.ema1 - saver.ema2);
		} else {
			// EMAtoday=α * ( Pricetoday - EMAyesterday ) +
			// EMAyesterday;
			saver.ema1 = (ALPHA1 * (saver.lastPrice) + (1 - ALPHA1) * pSaver.ema1);
			saver.ema2 = (ALPHA2 * (saver.lastPrice) + (1 - ALPHA2) * pSaver.ema2);
			saver.dif = (saver.ema1 - saver.ema2);
		}
	}

	public static void main(String[] args) throws Exception {
		System.out.println(System.currentTimeMillis());
		Timer timer = new Timer();
		timer.schedule(new Task(), 1000*60*30);

		long logTime = System.currentTimeMillis();
		File file = new File("C:\\Users\\QiLIn\\Desktop\\out" + logTime + ".txt");
		if (file.exists())
			file.delete();
		file.createNewFile();
		FileOutputStream fos = new FileOutputStream(file);
		OutputStreamWriter osw = new OutputStreamWriter(fos);
		BufferedWriter bw = new BufferedWriter(osw);

		bg();

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

		Saver pSaver1minute = new Saver();
		Saver pSaver15minute = new Saver();
		Saver pSaver1day = new Saver();
		Saver pMax1minute = new Saver();
		Saver pMax15minute = new Saver();
		Saver pMax1day = new Saver();
		Saver pMin1minute = new Saver();
		Saver pMin15minute = new Saver();
		Saver pMin1day = new Saver();
		Saver saver1minute = new Saver();
		Saver saver15minute = new Saver();
		Saver saver1day = new Saver();
		Tab tab = new Tab();
		int beforeMinute = 0; // 编译器要求
		int currentMinute = 0; // 编译器要求
		char[] temp;
		int tempInt;
		ExecutorService executor = Executors.newFixedThreadPool(2);
		Callable<Integer> readTask = new Callable<Integer>() {
			@Override
			public Integer call() throws Exception {
				return System.in.read();
			}
		};
		int codeCount = 1;
		int coooococ = 0;
		String bgDate, edDate;
		for (String code : codes) {
			if(coooococ >= 10)
				break;
			coooococ++;
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

			if (testAndDelete(code, bgDate, edDate) == false) {
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

			pSaver1minute.initPSaver(conn, tableName1);
			pSaver15minute.initPSaver(conn, tableName2);
			pSaver1day.initPSaver(conn, tableName3);
			pMax1minute.initPMax(conn_A, "pd_1minute", "d_1minute", code, pSaver1minute);
			pMax15minute.initPMax(conn_A, "pd_15minute", "d_15minute", code, pSaver15minute);
			pMax1day.initPMax(conn_A, "pd_1day", "d_1day", code, pSaver1day);
			pMin1minute.initPMin(conn_A, "pd_1minute", "d_1minute", code, pSaver1minute);
			pMin15minute.initPMin(conn_A, "pd_15minute", "d_15minute", code, pSaver15minute);
			pMin1day.initPMin(conn_A, "pd_1day", "d_1day", code, pSaver1day);
			while (dates.isEmpty() == false) {
				String date = dates.removeFirst();
				String dateNum = TypeConver.getDateNum(date);
				String tableName = "tab_" + dateNum + fullCode;
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

				String fullDate = TypeConver.getFullDate(date);

				saver1minute.initSaver();
				saver15minute.initSaver();
				saver1day.initSaver();
				saver1minute.date = fullDate;
				saver15minute.date = fullDate;
				saver1day.date = fullDate;
				ps = conn.prepareStatement("SELECT price, num, tim FROM " + tableName
						+ " WHERE tim>='09:30:00' AND tim<='15:00:00' ORDER BY tim ASC");
				rs = ps.executeQuery();
				if (rs.next() == false) {
					rs.close();
					ps.close();
					continue;
				}
				tab.currentPrice = (rs.getDouble(1));
				tab.num = (rs.getInt(2));
				tab.time = Arrays.copyOf((rs.getString(3)).substring(0, 5).toCharArray(), 5);
				temp = Arrays.copyOf(tab.time, 5);
				saver1minute.time = Arrays.copyOf(tab.time, 5);
				tempInt = (temp[3] - '0') * 10 + temp[4] - '0';
				beforeMinute = tempInt;
				tempInt = (int) (tempInt / 15 * 15);
				temp[3] = (char) (tempInt / 10 + '0');
				temp[4] = (char) (tempInt % 10 + '0');
				saver15minute.time = Arrays.copyOf(temp, 5);
				saver1day.time = "00:00".toCharArray();
				saver1minute.accum(tab);
				while (rs.next()) {
					tab.currentPrice = (rs.getDouble(1));
					tab.num = (rs.getInt(2));
					tab.time = Arrays.copyOf((rs.getString(3)).substring(0, 5).toCharArray(), 5);
					currentMinute = (tab.time[3] - '0') * 10 + tab.time[4] - '0';

					if ((currentMinute) == (beforeMinute)) {
						saver1minute.accum(tab);
					} else {
						{
							// 第一个储存
							setCens(pSaver1minute, saver1minute);

							////////////////////////////////
							addBatch(psBatch1, saver1minute.date, String.valueOf(saver1minute.time),
									saver1minute.lowPrice, saver1minute.highPrice, saver1minute.lastPrice,
									saver1minute.avgSummer / saver1minute.sum, saver1minute.sum, saver1minute.ema1,
									saver1minute.ema2, saver1minute.dif);
									////////////////////////////////

							///////////////////////////////////////////////////////////
							if (pSaver1minute.prePrice >= 0) {
								judgePole(pSaver1minute, saver1minute);

								judgeDevia(pSaver1minute, pMin1minute, pMax1minute, code, psBatch4, psBatch7);
							}

							saver1minute.exists = (true);
							if (pSaver1minute.exists == true) {
								pSaver1minute.prePrice = (pSaver1minute.lastPrice);
							}
							pSaver1minute.cloneExceptPrepri(saver1minute);
							///////////////////////////////////////////////////////////

							// 更新：
							saver15minute.accum(saver1minute);
						}
						if ((currentMinute) / 15 != (beforeMinute) / 15) {
							// 第二个储存
							setCens(pSaver15minute, saver15minute);

							/////////////////////////////////////////////
							addBatch(psBatch2, saver15minute.date, String.valueOf(saver15minute.time),
									saver15minute.lowPrice, saver15minute.highPrice, saver15minute.lastPrice,
									saver15minute.avgSummer / saver15minute.sum, saver15minute.sum, saver15minute.ema1,
									saver15minute.ema2, saver15minute.dif);
									/////////////////////////////////////////////

							//////////////////////////////////////////////////////////////
							if (pSaver15minute.prePrice >= 0) {
								judgePole(pSaver15minute, saver15minute);

								judgeDevia(pSaver15minute, pMin15minute, pMax15minute, fullCode, psBatch5, psBatch8);
							}

							saver15minute.exists = (true);
							if (pSaver15minute.exists == true) {
								pSaver15minute.prePrice = (pSaver15minute.lastPrice);
							}
							pSaver15minute.cloneExceptPrepri(saver15minute);
							//////////////////////////////////////////////////////////////

							saver1day.accum(saver15minute);

							saver15minute.initSaver();
							saver15minute.time = Arrays.copyOf(tab.time, 5);
							tempInt = (int) (currentMinute / 15 * 15);
							saver15minute.time[3] = (char) (tempInt / 10 + '0');
							saver15minute.time[4] = (char) (tempInt % 10 + '0');
						}
						saver1minute.initSaver();
						saver1minute.accum(tab);
						saver1minute.time = Arrays.copyOf(tab.time, 5);
						beforeMinute = currentMinute;
					}
				}
				rs.close();
				ps.close();

				{
					// 第一个储存
					setCens(pSaver1minute, saver1minute);

					////////////////////////////////
					addBatch(psBatch1, saver1minute.date, String.valueOf(saver1minute.time), saver1minute.lowPrice,
							saver1minute.highPrice, saver1minute.lastPrice, saver1minute.avgSummer / saver1minute.sum,
							saver1minute.sum, saver1minute.ema1, saver1minute.ema2, saver1minute.dif);
							////////////////////////////////

					///////////////////////////////////////////////////////////
					if (pSaver1minute.prePrice >= 0) {
						judgePole(pSaver1minute, saver1minute);

						judgeDevia(pSaver1minute, pMin1minute, pMax1minute, fullCode, psBatch4, psBatch7);
					}

					saver1minute.exists = (true);
					if (pSaver1minute.exists == true) {
						pSaver1minute.prePrice = (pSaver1minute.lastPrice);
					}
					pSaver1minute.cloneExceptPrepri(saver1minute);
					///////////////////////////////////////////////////////////

					// 更新：
					saver15minute.accum(saver1minute);
				}
				{
					// 第二个储存
					setCens(pSaver15minute, saver15minute);

					/////////////////////////////////////////////
					addBatch(psBatch2, saver15minute.date, String.valueOf(saver15minute.time), saver15minute.lowPrice,
							saver15minute.highPrice, saver15minute.lastPrice,
							saver15minute.avgSummer / saver15minute.sum, saver15minute.sum, saver15minute.ema1,
							saver15minute.ema2, saver15minute.dif);
							/////////////////////////////////////////////

					//////////////////////////////////////////////////////////////
					if (pSaver15minute.prePrice >= 0) {
						judgePole(pSaver15minute, saver15minute);

						judgeDevia(pSaver15minute, pMin15minute, pMax15minute, fullCode, psBatch5, psBatch8);
					}

					saver15minute.exists = (true);
					if (pSaver15minute.exists == true) {
						pSaver15minute.prePrice = (pSaver15minute.lastPrice);
					}
					pSaver15minute.cloneExceptPrepri(saver15minute);
					//////////////////////////////////////////////////////////////

					// 更新
					saver1day.accum(saver15minute);
				}
				{
					// 第三个储存
					setCens(pSaver1day, saver1day);

					/////////////////////////////////////////////
					addBatch(psBatch3, saver1day.date, saver1day.lowPrice, saver1day.highPrice, saver1day.lastPrice,
							saver1day.avgSummer / saver1day.sum, saver1day.sum, saver1day.ema1, saver1day.ema2,
							saver1day.dif);
							/////////////////////////////////////////////

					//////////////////////////////////////////////////////////////
					if (pSaver1day.prePrice >= 0) {
						judgePole(pSaver1day, saver1day);

						judgeDevia(pSaver1day, pMin1day, pMax1day, fullCode, psBatch6, psBatch9);
					}

					saver1day.exists = (true);
					if (pSaver1day.exists == true) {
						pSaver1day.prePrice = (pSaver1day.lastPrice);
					}
					pSaver1day.cloneExceptPrepri(saver1day);
					//////////////////////////////////////////////////////////////
				}
			}

			edTable();

//			ps = conn.prepareStatement(
//					"INSERT INTO tabMgrHistory SELECT * FROM tabMgr WHERE cod=? AND dat>=? AND dat<=?");
//			ps.setString(1, code);
//			ps.setString(2, bgDate);
//			ps.setString(3, edDate);
//			ps.executeUpdate();
//			ps.close();
//			ps = conn.prepareStatement("DELETE FROM tabMgr WHERE cod=? AND dat>=? AND dat<=?");
//			ps.setString(1, code);
//			ps.setString(2, bgDate);
//			ps.setString(3, edDate);
//			ps.executeUpdate();
//			ps.close();

			bw.write("code: " + code + (codeCount++) + "\r\n");
			bw.flush();
		}

		conn.close();

		bw.write("used time: " + (System.currentTimeMillis() - logTime));
		bw.flush();
		fos.close();

		System.out.println(System.currentTimeMillis());
	}

	private static void openBatch(String tableName1, String tableName2, String tableName3) throws SQLException {
		psBatch1 = connBatch1.prepareStatement("INSERT INTO " + tableName1 + " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch2 = connBatch2.prepareStatement("INSERT INTO " + tableName2 + " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch3 = connBatch3.prepareStatement("INSERT INTO " + tableName3 + " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch4 = connBatch4.prepareStatement("INSERT INTO pd_1minute VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch5 = connBatch5
				.prepareStatement("INSERT INTO pd_15minute VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch6 = connBatch6.prepareStatement("INSERT INTO pd_1day VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch7 = connBatch7.prepareStatement("INSERT INTO d_1minute VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch8 = connBatch8.prepareStatement("INSERT INTO d_15minute VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		psBatch9 = connBatch9.prepareStatement("INSERT INTO d_1day VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	}

	private static void createColTable(String tableName1, String tableName2, String tableName3) {
		try {
			String sql = "CREATE TABLE " + tableName1
					+ " ([date] DATE, [time] TIME(0), lowPrice SMALLMONEY, highPrice SMALLMONEY, price SMALLMONEY, avgPrice SMALLMONEY, num INT, ema1 SMALLMONEY, ema2 SMALLMONEY, dif DECIMAL(15, 6), PRIMARY KEY([date] ASC, [time] ASC))";
			ps = conn.prepareStatement(sql);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
		try {
			String sql = "CREATE TABLE " + tableName2
					+ " ([date] DATE, [time] TIME(0), lowPrice SMALLMONEY, highPrice SMALLMONEY, price SMALLMONEY, avgPrice SMALLMONEY, num INT, ema1 SMALLMONEY, ema2 SMALLMONEY, dif DECIMAL(15, 6), PRIMARY KEY([date] ASC, [time] ASC))";
			ps = conn.prepareStatement(sql);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
		try {
			String sql = "CREATE TABLE " + tableName3
					+ " ([date] DATE, lowPrice SMALLMONEY, highPrice SMALLMONEY, price SMALLMONEY, avgPrice SMALLMONEY, num INT, ema1 SMALLMONEY, ema2 SMALLMONEY, dif DECIMAL(15, 6), PRIMARY KEY([date] ASC))";
			ps = conn.prepareStatement(sql);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException e) {
		}
	}

	// [date] DATE, [time] TIME(0), price SMALLMONEY, num INT, dif DECIMAL(15,
	// 6), ema1 SMALLMONEY, ema2 SMALLMONEY, highPrice SMALLMONEY, lowPrice
	// SMALLMONEY, avgPrice SMALLMONEY
	public static void addBatch(PreparedStatement ps, String date, String time, double lowPrice, double highPrice,
			double price, double avgPrice, int num, double ema1, double ema2, double dif) throws SQLException {
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

		ps.addBatch();
	}

	public static void addBatch(PreparedStatement ps, String date, double lowPrice, double highPrice, double price,
			double avgPrice, int num, double ema1, double ema2, double dif) throws SQLException {
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

		ps.addBatch();
	}

	static int batchCount = 800050;
	// code DECIMAL(6, 0), [date] DATE, [time] TIME(0), pole SMALLINT, devia
	// SMALLINT, price SMALLMONEY, num INT, dif DECIMAL(15, 6), ema1 SMALLMONEY,
	// ema2 SMALLMONEY, highPrice SMALLMONEY, lowPrice SMALLMONEY, avgPrice
	// SMALLMONEY
	public static void addBatch(PreparedStatement ps, String code, String date, String time, double lowPrice,
			double highPrice, double price, double avgPrice, int num, double ema1, double ema2, double dif, int pole,
			int devia) throws SQLException {
		if (pole == 0)
			return;
		// if (pole == -1)
		// pole = -1;
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

	public void initPSaver(Connection conn, String tableName) throws SQLException {
		String sqlStr = "SELECT TOP 2 * FROM " + tableName + " ORDER BY date DESC";
		if (tableName.endsWith("_1day") == false) {
			sqlStr += ", time DESC";
		}
		PreparedStatement ps = conn.prepareStatement(sqlStr);
		ResultSet rs = ps.executeQuery();
		if (rs.next() == false) {
			rs.close();
			ps.close();
			prePrice = -1;
			exists = false;
			return;
		}
		lowPrice = rs.getDouble("lowPrice");
		highPrice = rs.getDouble("highPrice");
		lastPrice = rs.getDouble("price");
		sum = rs.getInt("num");
		date = rs.getString("date");
		if (tableName.endsWith("_1day") == false) {
			time = rs.getString("time").substring(0, 5).toCharArray();
		} else {
			time = "00:00".toCharArray();
		}
		double avgPrice = rs.getDouble("avgPrice");
		avgSummer = sum * avgPrice;
		ema1 = rs.getDouble("ema1");
		ema2 = rs.getDouble("ema2");
		dif = rs.getDouble("dif");

		exists = true;

		if (rs.next() == false) {
			prePrice = -1;
		} else {
			prePrice = rs.getDouble("price");
		}

		rs.close();
		ps.close();
	}

	public void initPMin(Connection conn, String tableName, String tableNameD, String code, Saver pSaver)
			throws SQLException {
		PreparedStatement ps = conn.prepareStatement(
				"SELECT TOP 2 * FROM " + tableName + " WHERE code=? and pole=1 ORDER BY date DESC, time DESC");
		ps.setString(1, code);
		ResultSet rs = ps.executeQuery();
		if (rs.next() == false) {
			rs.close();
			ps.close();
			lastPrice = -1;
			return;
		}
		this.date = rs.getString("date");
		this.time = rs.getString("time").substring(0, 5).toCharArray();
		lastPrice = rs.getDouble("price");

		dif = rs.getDouble("dif");
		devia = rs.getInt("devia");

		if (this.date.equals(pSaver.date) && this.time.equals(pSaver.time)) {
			double temp = devia;
			if (rs.next() == false) {
				lastPrice = -1;
			} else {
				lastPrice = rs.getDouble("price");

				dif = rs.getDouble("dif");
				devia = rs.getInt("devia");
			}
			rs.close();
			ps.close();

//			ps = conn.prepareStatement("DELETE FROM " + tableName + " WHERE code=? AND date=? AND time=?");
//			ps.setString(1, code);
//			ps.setString(2, pSaver.date);
//			ps.setString(3, new String(pSaver.time));
//			ps.executeUpdate();
//			ps.close();
//
//			if (temp == 0) {
//				return;
//			}
//
//			ps = conn.prepareStatement("DELETE FROM " + tableNameD + " WHERE code=? AND date=? AND time=?");
//			ps.setString(1, code);
//			ps.setString(2, pSaver.date);
//			ps.setString(3, new String(pSaver.time));
//			ps.executeUpdate();
//			ps.close();

		} else {
			rs.close();
			ps.close();
		}
	}

	public void initPMax(Connection conn, String tableName, String tableNameD, String code, Saver pSaver)
			throws SQLException {
		PreparedStatement ps = conn.prepareStatement(
				"SELECT TOP 2 * FROM " + tableName + " WHERE code=? and pole=-1 ORDER BY date DESC, time DESC");
		ps.setString(1, code);
		ResultSet rs = ps.executeQuery();
		if (rs.next() == false) {
			rs.close();
			ps.close();
			lastPrice = 99999999;
			return;
		}
		this.date = rs.getString("date");
		this.time = rs.getString("time").substring(0, 5).toCharArray();
		lastPrice = rs.getDouble("price");

		dif = rs.getDouble("dif");
		devia = rs.getInt("devia");

		if (this.date.equals(pSaver.date) && this.time.equals(pSaver.time)) {
			double temp = devia;
			if (rs.next() == false) {
				lastPrice = 99999999;
			} else {
				lastPrice = rs.getDouble("price");

				dif = rs.getDouble("dif");
				devia = rs.getInt("devia");
			}
			rs.close();
			ps.close();
//			ps = conn.prepareStatement("DELETE FROM " + tableName + " WHERE code=? AND date=? AND time=?");
//			ps.setString(1, code);
//			ps.setString(2, pSaver.date);
//			ps.setString(3, new String(pSaver.time));
//			ps.executeUpdate();
//			ps.close();
//
//			if (temp == 0) {
//				return;
//			}
//
//			ps = conn.prepareStatement("DELETE FROM " + tableNameD + " WHERE code=? AND date=? AND time=?");
//			ps.setString(1, code);
//			ps.setString(2, pSaver.date);
//			ps.setString(3, new String(pSaver.time));
//			ps.executeUpdate();
//			ps.close();

		} else {
			rs.close();
			ps.close();
		}
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

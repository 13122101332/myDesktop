package com.devia.controller;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.devia.dao.DBOperation;
import com.devia.model.SelectStock;

public class initPage extends HttpServlet{
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws IOException, ServletException {
		DBOperation dbo = new DBOperation();
		String date1 = request.getParameter("date1");
		String date2 = request.getParameter("date2");
		String sortdate = request.getParameter("sortdate");
		String code1 = request.getParameter("code1");
		String code2 = request.getParameter("code2");
		String sortcode = request.getParameter("sortcode");
		String devia1 = request.getParameter("devia1");
		String devia2 = request.getParameter("devia2");
		String sortdevia = request.getParameter("sortdevia");
		String tabname = request.getParameter("type");

		// 若第二个输入为空，则将其置为第一个输入
		if ((date2 == null || date2 == "") && (date1 != null || date1 != "")) {
			date2 = date1;
		}
		if ((code2 == null || code2 == "") && (code1 != null || code1 != "")) {
			code2 = code1;
		}
		if ((devia2 == null || devia2 == "")
				&& (devia1 != null || devia1 != "")) {
			devia2 = devia1;
		}

		System.out.println("date1="+date1+" ;date2=="+date2+" ;code1=="+code1);
		request.getSession().setAttribute("date1", date1);
		request.getSession().setAttribute("date2", date2);
		request.getSession().setAttribute("sortdate", sortdate);
		request.getSession().setAttribute("code1", code1);
		request.getSession().setAttribute("code2", code2);
		request.getSession().setAttribute("sortcode", sortcode);
		request.getSession().setAttribute("devia1", devia1);
		request.getSession().setAttribute("devia2", devia2);
		request.getSession().setAttribute("sortdevia", sortdevia);
		request.getSession().setAttribute("tabname", tabname);

		int cur = 1;// 初始页码为1
		int totalPage = dbo.getTotalPage(date1, date2, code1, code2, devia1, devia2, tabname);// 总页数
		request.getSession().setAttribute("cur", cur);
		request.getSession().setAttribute("totalPage", totalPage);
		
		System.out.println("initPage--cur:"+cur+" ;totalPage:"+totalPage);
	
		List<SelectStock> list = new ArrayList<SelectStock>();
		list = dbo.getDate(date1, date2, sortdate, code1, code2, sortcode,
				devia1, devia2, sortdevia, tabname, cur);
		request.getSession().setAttribute("list", list);
		response.sendRedirect("index.jsp");
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws IOException, ServletException {
		doGet(request, response);
	}
}
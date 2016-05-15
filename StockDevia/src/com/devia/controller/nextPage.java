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

public class nextPage extends HttpServlet {
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws IOException, ServletException {
		DBOperation dbo = new DBOperation();
		String date1 = (String) request.getSession().getAttribute("date1");
		String date2 = (String) request.getSession().getAttribute("date2");
		String sortdate = (String) request.getSession().getAttribute("sortdate");
		String code1 = (String) request.getSession().getAttribute("code1");
		String code2 = (String) request.getSession().getAttribute("code2");
		String sortcode = (String) request.getSession().getAttribute("sortcode");
		String devia1 = (String) request.getSession().getAttribute("devia1");
		String devia2 = (String) request.getSession().getAttribute("devia2");
		String sortdevia = (String) request.getSession().getAttribute("sortdevia");
		String tabname = (String) request.getSession().getAttribute("tabname");

		int cur = Integer.parseInt(request.getParameter("cur"));// Ò³Âë
		int totalPage = dbo.getTotalPage(date1, date2, code1, code2, devia1, devia2, tabname);// ×ÜÒ³Êý

		if (cur < 1)
			cur = 1;
		if (cur > totalPage)
			cur = totalPage;
		request.getSession().setAttribute("cur", cur);
		request.getSession().setAttribute("totalPage", totalPage);
		System.out.println("nextPage--cur:"+cur+" ;totalPage:"+totalPage);
		
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

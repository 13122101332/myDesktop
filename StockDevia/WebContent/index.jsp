<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>index.jsp</title>
</head>
<style type="text/css">
input {
	height: 22px;
	width: 140px;
}
#selectForm {
	background-color: #D9EFF2;
	position: absolute;
	width: 1075px;
	height: 231px;
	left: 19px;
}
#con {
	background-color: #FFFFFF;
	position: absolute;
	left: 19px;
	top: 243px;
	width: 1075px;
}
</style>
<body>
	<div id="selectForm" >
		<center><h3>请输入查询条件：</h3></center>
		<form method="post" action="search">
			<table width="581" height="138" align="center">
				<tr>
					<td align="right">日期范围：</td>
					<td><input type="date" name="date1">--<input
						type="date" name="date2"></td>
					<td align="right">排序类型：</td>
					<td><select name="sortdate">
							<option value="desc">降序</option>
							<option value="asc">升序</option>
					</select></td>
				</tr>
				<tr>
					<td align="right">股票代码：</td>
					<td><input type="number" name="code1">--<input
						type="number" name="code2"></td>
					<td align="right">排序类型：</td>
					<td><select name="sortcode">
							<option value="desc">降序</option>
							<option value="asc">升序</option>
					</select></td>
				</tr>

				<tr>
					<td align="right">背离值：</td>
					<td><input type="number" name="devia1">--<input
						type="number" name="devia2"></td>
					<td align="right">排序类型：</td>
					<td><select name="sortdevia">
							<option value="desc">降序</option>
							<option value="asc">升序</option>
					</select></td>
				</tr>
				<tr>
					<td align="right">周期：</td>
					<td><select name="type">
							<option value="d_1minute">1分钟</option>
							<option value="d_15minute">15分钟</option>
							<option value="d_1day">1天</option>
					</select></td>
				</tr>
				<tr>
					<td colspan="4" align="center">
					<input type="submit" value="提交"></td>
				</tr>
		  </table>
		</form>
</div>

    <div id="con">
    <table width="1075" border cellspacing="0">
    <tr align="center"  bgcolor="#D9EFF2">
        <td>股票代码</td>
        <td>日期</td>
        <td>背离值</td>
        <td>时间</td>
        <td>收盘价</td>
        <td>最低价</td>
        <td>最高价</td>
        <td>平均价格</td>
        <td>成交数量</td>
        <td>ema1</td>
        <td>ema2</td>
        <td>dif</td>  
    </tr>
   
    <c:forEach items="${list}" var="p">
     <tr align="center">
        <td>${p.code}</td>
        <td>${p.date}</td>
        <td>${p.devia}</td>
        <td>${p.time}</td>
        <td>${p.price}</td>
        <td>${p.lowprice}</td>
        <td>${p.highprice}</td>
        <td>${p.avgprice}</td>
        <td>${p.num}</td>
        <td>${p.ema1}</td>
        <td>${p.ema2}</td>
        <td>${p.dif}</td>
      </tr>
    </c:forEach>
    
    </table>
	<p>
	<p align="center">
	<a href="pageAction?cur=1">首页</a>  
	<a href="pageAction?cur=${cur-1}">上一页</a>
	 ${cur}
	<a href="pageAction?cur=${cur+1}">下一页</a>
	<a href="pageAction?cur=${totalPage}">尾页</a> 
	共${totalPage }页
	</p>
</div>
<div style="background-color:#C1E4EA; width:1075px; height:30; top:705px; position:absolute; left: 19px;">
&nbsp;
</div>
</body>
</html>
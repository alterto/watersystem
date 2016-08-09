package com.login;

import com.jfinal.core.Controller;

public class LoginController extends Controller {
	
	// 登录页面
	public void index()
	{
		render("index.html");
	}
	
	// 登录判断
	public void check()
	{
		String strUserName = getPara("username");
		String strPassword = getPara("password");
		renderText("true");
		setSessionAttr("username", strUserName);
	}
}

package com.global;

import java.util.ResourceBundle.Control;

import com.index.IndexController;
import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;
import com.jfinal.core.Controller;
import com.login.LoginController;

public class GlobalInterceptor implements Interceptor{

	public void intercept(Invocation arg0) {
		// TODO Auto-generated method stub
		
		// 拦截判断是否已经登录
		String strUserName = arg0.getController().getSessionAttr("username");
		if(strUserName == null
			&& arg0.getController().getClass()!= LoginController.class)
		{
			arg0.getController().redirect("/login");
		}else{
			if(strUserName != null
				&&arg0.getController().getClass() == LoginController.class)
			{
				arg0.getController().redirect("/");
			}else
				arg0.invoke();
		}
		
	}
}

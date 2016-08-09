package com.common.config;

import com.common.model._MappingKit;
import com.global.GlobalInterceptor;
import com.index.IndexController;
import com.jfinal.config.Constants;
import com.jfinal.config.Handlers;
import com.jfinal.config.Interceptors;
import com.jfinal.config.JFinalConfig;
import com.jfinal.config.Plugins;
import com.jfinal.config.Routes;
import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.ActiveRecordPlugin;
import com.jfinal.plugin.activerecord.dialect.SqlServerDialect;
import com.jfinal.plugin.c3p0.C3p0Plugin;
import com.jfinal.render.ViewType;
import com.login.LoginController;
import com.org.OrgController;
import com.power.RoleController;
import com.power.UserController;


public class Config extends JFinalConfig {

	@Override
	public void configConstant(Constants arg0) {
		// TODO Auto-generated method stub
		//设置为开发模式
		PropKit.use("a_little_config.txt");
		arg0.setDevMode(PropKit.getBoolean("devMode", false));
		arg0.setError404View("/error/404.html");
		arg0.setViewType(ViewType.JSP);
	}

	@Override
	public void configHandler(Handlers arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void configInterceptor(Interceptors arg0) {
		// TODO Auto-generated method stub
		arg0.add(new GlobalInterceptor());
	}

	@Override
	public void configPlugin(Plugins arg0) {
		// TODO Auto-generated method stub
		C3p0Plugin cp = new C3p0Plugin(PropKit.get("jdbcUrl"),
				PropKit.get("user"),
				PropKit.get("password"),
				PropKit.get("driver"));
		arg0.add(cp);
		ActiveRecordPlugin arp = new ActiveRecordPlugin(cp);
		arg0.add(arp);
		
		// 配置Sqlserver方言
		arp.setDialect(new SqlServerDialect());
		// 添加模板对象映射
		_MappingKit.mapping(arp);
	}

	@Override
	public void configRoute(Routes arg0) {
		// TODO Auto-generated method stub
		arg0.add("/", IndexController.class,"/index");
		arg0.add("/login", LoginController.class,"/login");
		arg0.add("/role", RoleController.class,"/power");
		arg0.add("/user",UserController.class,"/power");
		arg0.add("/org", OrgController.class,"/org");
		
	}
}

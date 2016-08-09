package com.power;

import java.sql.SQLException;
import java.util.List;
import com.common.model.Role;
import com.jfinal.core.Controller;
import com.jfinal.kit.JsonKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.IAtom;
import com.jfinal.plugin.activerecord.Page;


public class RoleController extends Controller {	
	
	// 角色管理
	public void index()
	{
		render("role.html");
	}
	
	// 角色数据表加载
	public void load()
	{
		int iPage = getParaToInt("page");
		int iRows = getParaToInt("rows");	
		
		Page<Role> rolePage = Role.dao.paginate(iPage, iRows, "SELECT RoleID,RoleName,Remark,CreateTime", "FROM t_Role");
		List<Role> list = rolePage.getList();
		
		renderJson(JsonKit.toJson(list));
	}
	
	// 角色保存更新
	public void save()
	{
		String strRoleID = getPara("RoleID");
		boolean ret = false;
		Role role = getModel(Role.class);
		if(strRoleID!=null)
		{	
			role.setRoleID(Integer.parseInt(strRoleID));
			ret = role.update();
		}else{
			ret = role.save();
		}

		if(ret)
		{
			renderHtml("{\"success\":\"true\"}");
		}else{
			renderHtml("{\"msg\":\"保存失败.\"}");
		}
	}
	
	// 角色删除
	public void remove()
	{
		boolean ret = Db.tx(new IAtom(){
			public boolean run() throws SQLException {
				// TODO Auto-generated method stub
				String strRoleIDArray = getPara("id");
				String []arr = strRoleIDArray.split(",");
				for(int index = 0;index<arr.length;index++)
				{
					if(!Role.dao.deleteById(Integer.parseInt(arr[index])))
					{
						return false;
					}
				}
				return true;
			}
		});
		
		if(ret)
		{
			renderJson("{\"success\":\"true\"}");
		}else{
			renderJson("{\"msg\":\"删除失败.\"}");
		}
	}
	
	// 角色列表
	public void list()
	{
		List<Role> list = Role.dao.find("SELECT RoleName,RoleID FROM t_Role");
		renderJson(JsonKit.toJson(list));
	}
}

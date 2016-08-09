package com.power;

import java.sql.SQLException;
import java.util.List;

import org.apache.commons.codec.digest.DigestUtils;

import com.common.model.User;
import com.jfinal.core.Controller;
import com.jfinal.kit.JsonKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.IAtom;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;


public class UserController extends Controller {
	
	public void index()
	{
		render("user.jsp");
	}
	
	public void loaddata()
	{	
		int iPage = getParaToInt("page");
		int iRows = getParaToInt("rows");	
		
		Page<User> rolePage = User.dao.paginate(iPage, iRows, "SELECT dbo.t_User.UserID,dbo.t_User.Password,dbo.t_User.OrgCode,dbo.t_User.RoleID,dbo.t_User.RealName,dbo.t_User.Email,dbo.t_User.Tel,dbo.t_User.CreateTime,dbo.t_User.LastLoginTime,dbo.t_User.Remark,dbo.t_Role.RoleName","FROM dbo.t_User LEFT JOIN dbo.t_Role ON dbo.t_User.RoleID = dbo.t_Role.RoleID");
		
		List<User> list = rolePage.getList();
		renderJson(JsonKit.toJson(list));
	}
	
	public void save()
	{
		boolean bedit = getPara("isedit") != null;
		boolean ret = false;
		
		User user = getModel(User.class);
		
		List<Record> list = null;
		
		if(!bedit)
		{
			list = Db.find("SELECT UserID FROM t_User WHERE UserID = ?",user.getUserID());
			if(list.size()>0)
			{
				renderHtml("{\"msg\":\"当前用户已存在.\"}");
				return;
			}
		}
		
		String strMD5Password = DigestUtils.md5Hex(user.getPassword());
		user.setPassword(strMD5Password);
		
		if(bedit)
		{	
			ret = user.update();
		}else{

			ret = user.save();
		}
		

		if(ret)
		{
			renderHtml("{\"success\":\"true\"}");
		}else{
			renderHtml("{\"msg\":\"保存失败.\"}");
		}
	}
	
	public void remove()
	{
		boolean ret = Db.tx(new IAtom(){
			public boolean run() throws SQLException {
				// TODO Auto-generated method stub
				String strRoleIDArray = getPara("id");
				String []arr = strRoleIDArray.split(",");
				for(int index = 0;index<arr.length;index++)
				{
					if(!User.dao.deleteById(arr[index]))
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
}

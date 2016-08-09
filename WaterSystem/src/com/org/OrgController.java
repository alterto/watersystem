package com.org;
import java.util.List;
import com.common.model.Org;
import com.jfinal.core.Controller;
import com.jfinal.kit.JsonKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;

public class OrgController extends Controller {
	
	public void index()
	{
		render("org.html");
	}
	
	public void loaddata()
	{	
		String strOrgCode = getPara("id");
		List<Record> org = null;
		if(strOrgCode!=null)
		{
			org = Db.find("SELECT * FROM t_Org WHERE ParentCode = ? ORDER BY OrgCode",strOrgCode);
		}else{
			org = Db.find("SELECT * FROM t_Org WHERE ParentCode IS NULL ORDER BY OrgCode");
		}
		
		for(final Record orgItem : org)
		{
			orgItem.set("state", IsHasChild(orgItem.getStr("OrgCode"))?"closed":"open");
		}
		
		renderJson(JsonKit.toJson(org));
	}
	
	public boolean IsHasChild(String orgCode)
	{
		int iChildCounts = Db.find("SELECT TOP 1 OrgCode FROM t_Org WHERE ParentCode = ?",orgCode).size();
		return iChildCounts>0;
	}
	
	public void save()
	{
		// 判断是否已经存在OrgCode
		String strOrgID = getPara("OrgID");
		boolean ret = false;
		List<Record> list = null;
		
		Org org = getModel(Org.class);
		String strOrgCode = org.getOrgCode();
		
		if(strOrgID == null)
		{
			list = Db.find("SELECT OrgName FROM t_Org WHERE OrgCode = ?",strOrgCode);
		}else{
			list = Db.find("SELECT OrgName FROM t_Org WHERE OrgCode = ? AND OrgID != ?",strOrgCode,strOrgID);
		}
		
		if(list.size()>0)
		{
			renderHtml("{\"msg\":\"当前行政区代码已经被【"+list.get(0).getStr("OrgName")+"】占用.\"}");
			return;
		}
		
		list = Db.find("SELECT OrgCode FROM t_Org WHERE charindex(OrgCode,?)>0 AND OrgCode != ? ORDER BY 1 DESC",strOrgCode,strOrgCode);
		
		String strParentCode = null;
		int iGrade = list.size();
		if(iGrade>0)
		{
			strParentCode = list.get(0).getStr("OrgCode");
			org.setParentCode(strParentCode);
		}
		
		org.setOrgGrade(iGrade+1);
		
		if(strOrgID!=null)
		{	
			org.setOrgID(Integer.parseInt(strOrgID));
			ret = org.update();
		}else{
			ret = org.save();
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
		String strOrgID = getPara("id");
		if(Db.update("DELETE FROM t_Org WHERE OrgID = ? OR charindex((SELECT OrgCode FROM t_Org WHERE OrgID = ?),ParentCode)>0 ", strOrgID,strOrgID)>0)
		{
			renderJson("{\"success\":\"true\"}");
		}else{
			renderJson("{\"msg\":\"删除失败.\"}");
		}
	}
}

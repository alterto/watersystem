package com.common.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings("serial")
public abstract class BaseUser<M extends BaseUser<M>> extends Model<M> implements IBean {

	public void setUserID(java.lang.String UserID) {
		set("UserID", UserID);
	}

	public java.lang.String getUserID() {
		return get("UserID");
	}

	public void setPassword(java.lang.String Password) {
		set("Password", Password);
	}

	public java.lang.String getPassword() {
		return get("Password");
	}

	public void setOrgCode(java.lang.String OrgCode) {
		set("OrgCode", OrgCode);
	}

	public java.lang.String getOrgCode() {
		return get("OrgCode");
	}

	public void setRoleID(java.lang.Integer RoleID) {
		set("RoleID", RoleID);
	}

	public java.lang.Integer getRoleID() {
		return get("RoleID");
	}

	public void setRealName(java.lang.String RealName) {
		set("RealName", RealName);
	}

	public java.lang.String getRealName() {
		return get("RealName");
	}

	public void setEmail(java.lang.String Email) {
		set("Email", Email);
	}

	public java.lang.String getEmail() {
		return get("Email");
	}

	public void setTel(java.lang.String Tel) {
		set("Tel", Tel);
	}

	public java.lang.String getTel() {
		return get("Tel");
	}

	public void setCreateTime(java.util.Date CreateTime) {
		set("CreateTime", CreateTime);
	}

	public java.util.Date getCreateTime() {
		return get("CreateTime");
	}

	public void setLastLoginTime(java.util.Date LastLoginTime) {
		set("LastLoginTime", LastLoginTime);
	}

	public java.util.Date getLastLoginTime() {
		return get("LastLoginTime");
	}

	public void setRemark(java.lang.String Remark) {
		set("Remark", Remark);
	}

	public java.lang.String getRemark() {
		return get("Remark");
	}

}

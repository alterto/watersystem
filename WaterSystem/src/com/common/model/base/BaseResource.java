package com.common.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings("serial")
public abstract class BaseResource<M extends BaseResource<M>> extends Model<M> implements IBean {

	public void setResID(java.lang.Integer ResID) {
		set("ResID", ResID);
	}

	public java.lang.Integer getResID() {
		return get("ResID");
	}

	public void setParentID(java.lang.Integer ParentID) {
		set("ParentID", ParentID);
	}

	public java.lang.Integer getParentID() {
		return get("ParentID");
	}

	public void setResName(java.lang.String ResName) {
		set("ResName", ResName);
	}

	public java.lang.String getResName() {
		return get("ResName");
	}

	public void setRemark(java.lang.String Remark) {
		set("Remark", Remark);
	}

	public java.lang.String getRemark() {
		return get("Remark");
	}

}

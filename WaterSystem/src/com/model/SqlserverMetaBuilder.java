package com.model;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import javax.sql.DataSource;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.generator.MetaBuilder;
import com.jfinal.plugin.activerecord.generator.TableMeta;

public class SqlserverMetaBuilder extends MetaBuilder {

	//是否将不带前缀的表，加入的生成队列   默认true,就是所有表都会自动生成
	private boolean flag = true;


	public SqlserverMetaBuilder(DataSource dataSource) {
	super(dataSource);
	}


	public SqlserverMetaBuilder(DataSource dataSource,boolean flag) {
	super(dataSource);
	this.flag = flag;
	}

	protected void buildTableNames(List<TableMeta> ret) throws SQLException {
	ResultSet rs = dbMeta.getTables(conn.getCatalog(), null, null, new String[]{"TABLE", "VIEW"});
	while (rs.next()) {
	String tableName = rs.getString("TABLE_NAME");

	if (excludedTables.contains(tableName)) {
	System.out.println("Skip excluded table :" + tableName);
	}
	else {
	TableMeta tableMeta = new TableMeta();
	tableMeta.name = tableName;
	tableMeta.remarks = rs.getString("REMARKS");

	boolean tempFlag = false;
	// 移除表名前缀仅用于生成 modelName、baseModelName。tableMeta.name 表名自身不受影响
	if (removedTableNamePrefixes != null) {
	for (String prefix : removedTableNamePrefixes) {
	if (tableName.startsWith(prefix)) {
	tableName = tableName.replaceFirst(prefix, "");
	tempFlag = true;
	break;
	}
	}
	}
	if(flag || tempFlag){
	tableMeta.modelName = StrKit.firstCharToUpperCase(StrKit.toCamelCase(tableName));
	tableMeta.baseModelName = "Base" + tableMeta.modelName;
	ret.add(tableMeta);
	}
	}
	}
	rs.close();
	}
}

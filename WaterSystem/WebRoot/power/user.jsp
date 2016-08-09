<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<meta name="keywords" content="jquery,ui,easy,easyui,web">
	<meta name="description" content="easyui help you build your web page easily!">
	<title>角色管理</title>
	<link rel="stylesheet" type="text/css" href="/js/easyui/themes/IconExtension.css">
	<link rel="stylesheet" type="text/css" href="/js/easyui/themes/default/easyui.css">
	<link rel="stylesheet" type="text/css" href="/js/easyui/themes/icon.css">
	<link rel="stylesheet" type="text/css" href="/js/easyui/demo.css">
	<style type="text/css">
		#fm{
			margin:0;
			padding:10px 30px;
		}
		.ftitle{
			font-size:14px;
			font-weight:bold;
			color:#666;
			padding:5px 0;
			margin-bottom:10px;
			border-bottom:1px solid #ccc;
		}
		.fitem{
			margin-bottom:5px;
		}
		.fitem label{
			display:inline-block;
			width:80px;
		}
	</style>
	<script type="text/javascript" src="/js/easyui/jquery.min.js"></script>
	<script type="text/javascript" src="/js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="/js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript">
		var url;
		function add(){
			$('#dlg').dialog('open').dialog('setTitle','新增用户');
			$('#fm').form('clear');
			url = '/user/save';
		}
		function edit(){
			var row = $('#dg').datagrid('getSelected');
			if (row){
				$('#dlg').dialog('open').dialog('setTitle','编辑用户');
				var cust_row = {'user.UserID':row.UserID,
						'user.OrgCode':row.OrgCode,
						'user.RoleID':row.RoleID,
						'user.RealName':row.RealName,
						'user.Email':row.Email,
						'user.Tel':row.Tel,
						'user.Remark':row.Remark};
				$('#fm').form('load',cust_row);
				url = '/user/save?isedit=true';
				$("input[name='user.UserID']").attr("readonly","readonly");
				
			}else{
				$.messager.alert('警告','请选择要修改的用户信息!','warning');
			}
		}
		function save(){
			$('#fm').form('submit',{
				url: url,
				onSubmit: function(){
					return $(this).form('validate');
				},
				success: function(result){
					var result = eval('('+result+')');
					if (result.success){
						$('#dlg').dialog('close');		// close the dialog
						$('#dg').datagrid('reload');	// reload the user data
					} else {
						$.messager.show({
							title: '错误',
							msg: result.msg
						});
					}
				}
			});
		}
		function remove(){
			var idarray = new Array;
			var checkedRow = $('#dg').datagrid('getChecked');
			if(checkedRow.length === 0)
			{
				$.messager.alert('警告','当前未选中任何删除项!','warning');
				return;
			}
			
			for(var index = 0;index<checkedRow.length;index++)
			{
				idarray.push(checkedRow[index].UserID);	
			}
	 
			var ret = idarray.join(",");
			if (checkedRow){
				$.messager.confirm('Confirm','您确定要删除当前选中用户?',function(r){
					if (r){
						$.post('/user/remove',{"id":ret},function(result){
							if (result.success){
								$('#dg').datagrid('reload');	// reload the user data
							} else {
								$.messager.show({	// show error message
									title: '错误',
									msg: result.msg
								});
							}
						});
					}
				});
			}
		}
		
	</script>
</head>
<body>
	<table id="dg" title="角色信息" class="easyui-datagrid" 
			fit="true"
			url="/user/loaddata"
			toolbar="#toolbar" pagination="true"
			selectOnCheck="false" checkOnSelect="false"
			rownumbers="true" fitColumns="true" singleSelect="true"
			onClickRow="onClickRow">
		<thead>
			<tr>
				<th data-options="field:'ck',checkbox:true"></th>
				<th field="UserID" width="50" >用户名</th>
				<th field="RoleID" width="50" hidden/>
				<th field="RoleName" width="50" >角色</th>
				<th field="OrgCode" width="50" >行政区代码</th>
				<th field="RealName" width="50" >真实姓名</th>
				<th field="Email" width="50" >电子邮箱</th>
				<th field="Tel" width="50" >联系电话</th>
				<th field="CreateTime" width="50" >创建时间</th>
				<th field="LastLoginTime" width="50" >上次登录时间</th>
				<th field="Remark" width="50" >备注</th>
			</tr>
		</thead>
	</table>
	
	<div id="toolbar">
		<a href="#" class="easyui-linkbutton" iconCls="icon-add" plain="true" onclick="add()">添加</a>
		<a href="#" class="easyui-linkbutton" iconCls="icon-edit" plain="true" onclick="edit()">编辑</a>
		<a href="#" class="easyui-linkbutton" iconCls="icon-remove" plain="true" onclick="remove()">删除</a>
	</div>
	
	<div id="dlg" class="easyui-dialog" style="width:400px;height:380px;padding:10px 20px"
			closed="true" buttons="#dlg-buttons">
		<div class="ftitle">用户信息</div>
		<form id="fm" method="post" novalidate>
			<div class="fitem">
				<label>用户名:</label>
				<input id="userid" name="user.UserID" class="easyui-validatebox" required="true">
			</div>
			<div class="fitem">
				<label>密码:</label>
				<input name="user.Password" type="password" class="easyui-validatebox" required="true">
			</div>
			<div class="fitem">
				<label>行政区:</label>
				<input name="user.OrgCode" class="easyui-validatebox" required="true">
			</div>
			<div class="fitem">
				<label>角色:</label>
				<input name="user.RoleID" 
				   	data-options="valueField: 'RoleID',
    				textField: 'RoleName'"
    				class="easyui-combobox" required="true" url="/role/list">
			</div>
			<div class="fitem">
				<label>真实姓名:</label>
				<input name="user.RealName">
			</div>
			<div class="fitem">
				<label>电子邮箱:</label>
				<input name="user.Email">
			</div>
			<div class="fitem">
				<label>联系电话:</label>
				<input name="user.Tel">
			</div>
			<div class="fitem">
				<label>备注:</label>
				<input name="user.Remark">
			</div>
		</form>
	</div>
	
	<div id="dlg-buttons">
		<a href="#" class="easyui-linkbutton" iconCls="icon-ok" onclick="save()">提交</a>
		<a href="#" class="easyui-linkbutton" iconCls="icon-cancel" onclick="javascript:$('#dlg').dialog('close')">取消</a>
	</div>
</body>
</html>
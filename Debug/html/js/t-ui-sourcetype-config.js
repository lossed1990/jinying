var UISourcetypeConfig = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');
        var $m_ui_table = undefined;
        var $m_ui_btn_add = undefined;

        var $m_ui_input_name = undefined;

        var m_url_data = "sourcetype-all.zc"; //数据获取接口
        var m_url_add = "sourcetype-add.zc"; //数据新增接口
        var m_url_del = "sourcetype-del.zc"; //数据删除接口
        var m_tablecolumns =   [ { 
            "data": "name"
        },        {        
            "data":  null,
            "className":   "center",
            "defaultContent":   '<a id="delrow" href="#"><i class="fa fa-trash-o"></i>删除</a>'      
        }];

        //=====公共方法=====
        wnd.load = function() {
            loadUI();
        }

        //加载用户配置界面
        function loadUI() {
            var html_str =
                '<div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                    <h2>总览 <small>材料类别</small></h2>\
                    <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                    <p class="text-muted font-13 m-b-30">\
                        功能描述：此处展示了目前所有的材料类别信息，您可通过该页面进行修改与删除。\
                    </p>\
                    <table id="table_sourcetype" class="table table-striped table-bordered">\
                        <thead>\
                        <tr>\
                            <th>材料类别名称</th>\
                            <th>操作</th>\
                        </tr>\
                        </thead>\
                        <tbody>\
                        </tbody>\
                    </table>\
                    </div>\
                </div>\
            </div>\
            <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>新增 <small>材料类别</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：您可通过该页面新增材料类别。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">类别名称</label>\
                            <div class="col-md-11 col-sm-11 col-xs-12">\
                                <input id="input_sourcetype_name" type="text" class="form-control" placeholder="">\
                            </div>\
                            </div>\
                            <div class="ln_solid">\
                            </div>\
                            <div class="form-group">\
                            <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                <button id="submit_add_sourcetype" type="submit" class="btn btn-primary">新增</button>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            bingAddEvent();
            bingDeleteEvent();
            refreshData();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_table = $("#table_sourcetype");
            $m_ui_btn_add = $('#submit_add_sourcetype');
            $m_ui_input_name = $('#input_sourcetype_name');
        }

        //刷新用户数据
        function refreshData() {
            $m_ui_table.dataTable({   
                destroy: true, //可重新加载
                ajax:   function (data,  callback,  settings)  {    
                    //ajax请求数据  
                    $.ajax({    
                        type: "POST",
                        url: m_url_data,
                        cache:  false,
                        //禁用缓存  
                        data: JSON.stringify("{'get':'all'}"),
                        //传入组装的参数  
                        dataType: "json",
                        success: function (result)  {     //封装返回数据    
                            var  returnData  =   {};    
                            returnData.draw  =  data.draw; //这里直接自行返回了draw计数器,应该由后台返回       
                            returnData.recordsTotal  =  result.recordsTotal; //返回数据全部记录      
                            returnData.recordsFiltered  =  result.recordsFiltered; //后台不实现过滤功能，每次查询均视作全部结果     
                            returnData.data  =  result.data; //返回的数据列表  
                            callback(returnData);    
                        }  
                    });  
                },
                columns:  m_tablecolumns         
            });
        }

        //绑定新增按钮事件
        function bingAddEvent() {
            $m_ui_btn_add.on('click', function() {
                var requestParam;
                if (isNull($m_ui_input_name.val().trim())) {
                    toastr.error('类别名称不能为空！');
                    return;
                }

                requestParam = {
                    "name": $m_ui_input_name.val().trim()
                };

                $.ajax({
                    type: "POST",
                    url: m_url_add,
                    contentType: "application/json;charset=utf-8",
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function(result) {
                        if (result.ok == 0) {
                            toastr.success('材料类别[' + $m_ui_input_name.val().trim() + ']添加成功');
                            refreshData();
                            $m_ui_input_name.val("");
                        } else {
                            toastr.error('材料类别[' + $m_ui_input_name.val().trim() + ']添加失败,' + result.errorinfo);
                        }
                    },
                    error: function() {
                        toastr.error('材料类别添加失败,请检查服务器及网络后重试！');
                    }
                });
            });
        }

        //绑定删除按钮事件
        function bingDeleteEvent() {  
            $m_ui_table.on( 'click', 'a#delrow', function (even)  {        
                even.preventDefault; //方法阻止元素发生默认的行为（例如，当点击提交按钮时阻止对表单的提交）。  
                // 得到当前对象的值                 
                var  data = $m_ui_table.DataTable().row( $(this).parents('tr') ).data();  

                var textInfoStr = "确定要删除材料类别[" + data.name + "]吗?";
                swal({  
                        title: "提示",
                        text: textInfoStr,
                        type: "warning",
                        showCancelButton: true,
                        confirmButtonColor: "#DD6B55",
                        confirmButtonText: "确定删除",
                        cancelButtonText: "取消"
                    },
                    function(isConfirm) {  
                        if (isConfirm) {           
                            var strParam = {
                                "name": data.name
                            };         
                            $.ajax({               
                                url:  m_url_del,
                                type:   'POST',
                                dataType:   'json',
                                data:  JSON.stringify(strParam),
                                success:   function(result) {              
                                    if  (result.ok == 0) {  
                                        toastr.success('材料类别[' + data.name + ']删除成功');           
                                        refreshData();   
                                    } 
                                    else {
                                        toastr.error('材料类别[' + data.name + ']删除失败,' + result.errorinfo); 
                                    }              
                                },
                                error: function() {              
                                    toastr.error('材料类别删除失败,请检查服务器及网络后重试！');            
                                }           
                            })  
                        }
                    });
            }); 
        }

        return wnd;
    }
};

var g_oSourcetypeConfig = UISourcetypeConfig.createNew();
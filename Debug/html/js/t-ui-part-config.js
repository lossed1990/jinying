var UIPartConfig = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');
        var $m_ui_table = undefined;
        var $m_ui_btn_add = undefined;

        var $m_ui_input_name = undefined;
        var $m_ui_select_type = undefined;
        var $m_ui_input_price = undefined;

        var m_url_data = "part-all.zc"; //数据获取接口
        var m_url_add = "part-add.zc"; //数据新增接口
        var m_url_del = "part-del.zc"; //数据删除接口
        var m_tablecolumns =   [ { 
            "data": "name"
        },        {    
            "data": "type"
        },        {    
            "data": "price"
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
                    <h2>总览 <small>配件</small></h2>\
                    <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                    <p class="text-muted font-13 m-b-30">\
                        功能描述：此处展示了目前所有的配件信息，您可通过该页面进行修改与删除。\
                    </p>\
                    <table id="table_part" class="table table-striped table-bordered">\
                        <thead>\
                        <tr>\
                            <th>配件名称</th>\
                            <th>配件种类</th>\
                            <th>价格</th>\
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
                        <h2>新增 <small>配件</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：您可通过该页面新增配件。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">配件名称</label>\
                                <div class="col-md-3 col-sm-3 col-xs-12">\
                                    <input id="input_part_name" type="text" class="form-control" placeholder="">\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">配件种类</label>\
                                <div class="col-md-3 col-sm-3 col-xs-12">\
                                    <select id="select_part_type" class="form-control">\
                                    </select>\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">单价(元)</label>\
                                <div class="col-md-3 col-sm-3 col-xs-12">\
                                    <input id="input_part_price" type="text" class="form-control" placeholder="">\
                                </div>\
                            </div>\
                            <div class = "ln_solid" > \
                            </div>\
                            <div class = "form-group" > \
                                <div class = "col-md-9 col-sm-9 col-xs-12 col-md-offset-1" > \
                                    <button id = "submit_add_part" class = "btn btn-primary" >新增</button>\
                                </div > \
                            </div>\
                        </div > \
                    </div>\
                </div > \
            </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            bingAddEvent();
            bingDeleteEvent();
            refreshPartType();
            refreshData();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_table = $("#table_part");
            $m_ui_btn_add = $('#submit_add_part');

            $m_ui_input_name = $('#input_part_name');
            $m_ui_select_type = $('#select_part_type');
            $m_ui_input_price = $('#input_part_price');
        }

        function refreshPartType() {
            $.ajax({    
                type: "POST",
                url: "parttype-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        $m_ui_select_type.html("");
                        toastr.error('暂未配置配件种类信息，请前往配置！');
                        return;
                    }

                    var strHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        var strSubHtml = '<option>' + result.data[i].name + '</option>';
                        strHtml = strHtml + strSubHtml;
                    }
                    $m_ui_select_type.html(strHtml);
                },
                error: function() {
                    $m_ui_select_type.html("");
                    toastr.error('获取配件种类信息失败,请刷新重试！');
                }  
            });  
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
                    toastr.error('配件名称不能为空！');
                    return;
                }

                if (isNull($m_ui_select_type.val().trim())) {
                    toastr.error('尚未选择配件种类！');
                    return;
                }

                if (isNull($m_ui_input_price.val().trim())) {
                    toastr.error('价格值不能为空！');
                    return;
                }

                if (!isPrice($m_ui_input_price.val().trim())) {
                    toastr.error('配件价格输入不合法，仅能精确到小数点后两位！');
                    return;
                }

                requestParam = {
                    "name": $m_ui_input_name.val().trim(),
                    "type": $m_ui_select_type.val().trim(),
                    "price": parseFloat($m_ui_input_price.val().trim())
                };

                $.ajax({
                    type: "POST",
                    url: m_url_add,
                    contentType: "application/json;charset=utf-8",
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function(result) {
                        if (result.ok == 0) {
                            toastr.success('新配件添加成功');
                            refreshData();
                            $m_ui_input_name.val("");
                            $m_ui_input_price.val("");

                        } else {
                            toastr.error('新配件添加失败,' + result.errorinfo);
                        }
                    },
                    error: function() {
                        toastr.error('新配件添加失败,请检查服务器及网络后重试！');
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

                var textInfoStr = "确定要删除该条记录吗?";
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
                                "name": data.name,
                                "type": data.type
                            };         
                            $.ajax({               
                                url:  m_url_del,
                                type:   'POST',
                                dataType:   'json',
                                data:  JSON.stringify(strParam),
                                success:   function(result) {              
                                    if  (result.ok == 0) {  
                                        toastr.success('配件删除成功');           
                                        refreshData();   
                                    } 
                                    else {
                                        toastr.error('配件删除失败,' + result.errorinfo); 
                                    }              
                                },
                                error: function() {              
                                    toastr.error('配件删除失败,请检查服务器及网络后重试！');            
                                }           
                            })  
                        }
                    });
            }); 
        }

        return wnd;
    }
};

var g_oPartConfig = UIPartConfig.createNew();
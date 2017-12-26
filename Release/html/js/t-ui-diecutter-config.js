var UIDieCutterConfig = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');
        var $m_ui_table = undefined;
        var $m_ui_btn_add = undefined;
        var $m_ui_btn_setprice = undefined;

        var $m_ui_select_type = undefined;
        var $m_ui_input_len = undefined;
        var $m_ui_input_width = undefined;
        var $m_ui_input_height = undefined;

        var $m_ui_input_price = undefined;
        var $m_ui_input_price_box = undefined;

        var m_url_data = "diecutter-all.zc"; //数据获取接口
        var m_url_add = "diecutter-add.zc"; //数据新增接口
        var m_url_del = "diecutter-del.zc"; //数据删除接口
        var m_tablecolumns =   [ { 
            "data": "producttype" 
        },        {   
            "data": "type" 
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
                        <h2>总览 <small>刀模规格</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：此处展示了目前所有的刀模规格信息，您可通过该页面进行修改与删除。\
                        </p>\
                        <table id="table_diecutter" class="table table-striped table-bordered">\
                            <thead>\
                                <tr>\
                                    <th>刀模类型</th>\
                                    <th>刀模规格</th>\
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
                        <h2>新增 <small>刀模规格</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：您可通过该页面新增刀模规格。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">刀模类型</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <select id="select_type" class="form-control">\
                                    <option>手提袋</option>\
                                    <option>礼品盒</option>\
                                </select>\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">长(mm)</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_diecutter_len" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">宽(mm)</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_diecutter_width" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">高(mm)</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_diecutter_height" type="text" class="form-control" placeholder="">\
                            </div>\
                            </div>\
                            <div class="ln_solid">\
                            </div>\
                            <div class="form-group">\
                            <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                <button id="submit_add_diecutter" class="btn btn-primary">新增</button>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>\
            <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>定价 <small>刀模规格</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：此处用来设置修改刀模的成本价。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                                <label class="control-label col-md-2 col-sm-2 col-xs-12">手提袋刀模单价(元)</label>\
                                <div class="col-md-4 col-sm-4 col-xs-12">\
                                    <input id="input_diecutter_price" type="text" class="form-control" placeholder="">\
                                </div>\
                                <label class="control-label col-md-2 col-sm-2 col-xs-12">包装盒刀模单价(元)</label>\
                                <div class="col-md-4 col-sm-5 col-xs-12">\
                                    <input id="input_diecutter_price_box" type="text" class="form-control" placeholder="">\
                                </div>\
                            </div>\
                        <div class="ln_solid">\
                        </div>\
                        <div class="form-group">\
                        <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                            <button id="submit_set_diecutterprice" class="btn btn-primary">设置</button>\
                            </div>\
                        </div>\
                    </div>\
                    </div>\
                </div>\
            </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            bingPriceEvent();
            bingAddEvent();
            bingDeleteEvent();
            refreshData();
            getPrice();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_table = $("#table_diecutter");
            $m_ui_btn_add = $('#submit_add_diecutter');
            $m_ui_btn_setprice = $('#submit_set_diecutterprice');
            $m_ui_select_type = $('#select_type');
            $m_ui_input_len = $('#input_diecutter_len');
            $m_ui_input_width = $('#input_diecutter_width');
            $m_ui_input_height = $('#input_diecutter_height');
            $m_ui_input_price = $('#input_diecutter_price');
            $m_ui_input_price_box = $('#input_diecutter_price_box');
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
                if (isNull($m_ui_input_len.val().trim())) {
                    toastr.error('长度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_len.val().trim())) {
                    toastr.error('长度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_width.val().trim())) {
                    toastr.error('宽度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_width.val().trim())) {
                    toastr.error('宽度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_height.val().trim())) {
                    toastr.error('高度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_height.val().trim())) {
                    toastr.error('高度仅能输入数字！');
                    return;
                }

                requestParam = {
                    "producttype":$m_ui_select_type.val().trim(),
                    "l": parseInt($m_ui_input_len.val().trim()),
                    "w": parseInt($m_ui_input_width.val().trim()),
                    "h": parseInt($m_ui_input_height.val().trim()),
                };

                $.ajax({
                    type: "POST",
                    url: m_url_add,
                    contentType: "application/json;charset=utf-8",
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function(result) {
                        if (result.ok == 0) {
                            toastr.success('新刀模规格添加成功');
                            refreshData();
                            $m_ui_input_len.val("");
                            $m_ui_input_width.val("");
                            $m_ui_input_height.val("");
                        } else {
                            toastr.error('新刀模规格添加失败,' + result.errorinfo);
                        }
                    },
                    error: function() {
                        toastr.error('刀模规格添加失败,请检查服务器及网络后重试！');
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

                var textInfoStr = "确定要删除刀模规格[" + data.type + "]吗?";
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
                                "producttype": data.producttype,
                                "type": data.type
                            };         
                            $.ajax({               
                                url:  m_url_del,
                                type:   'POST',
                                dataType:   'json',
                                data:  JSON.stringify(strParam),
                                success:   function(result) {              
                                    if  (result.ok == 0) {  
                                        toastr.success('刀模规格[' + data.type + ']删除成功');           
                                        refreshData();   
                                    } 
                                    else {
                                        toastr.error('刀模规格[' + data.type + ']删除失败,' + result.errorinfo); 
                                    }              
                                },
                                error: function() {              
                                    toastr.error('刀模规格删除失败,请检查服务器及网络后重试！');            
                                }           
                            })  
                        }
                    });
            }); 
        }

        function getPrice() {
            $.ajax({    
                type: "POST",
                url: "diecutter-getprice.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    $m_ui_input_price.val(result.price);
                    $m_ui_input_price_box.val(result.boxprice);
                },
                error: function() {
                    toastr.error('获取刀模价格失败,请刷新重试！');
                }  
            });  
        }

        function bingPriceEvent() {
            $m_ui_btn_setprice.on('click', function() {
                if (isNull($m_ui_input_price.val().trim())) {
                    toastr.error('手提袋刀模单价不能为空！');
                    return;
                }

                if (!isPrice($m_ui_input_price.val().trim())) {
                    toastr.error('手提袋刀模价格不合法，仅能精确到小数点后两位！');
                    return;
                }

                if (isNull($m_ui_input_price_box.val().trim())) {
                    toastr.error('包装盒刀模单价不能为空！');
                    return;
                }

                if (!isPrice($m_ui_input_price_box.val().trim())) {
                    toastr.error('包装盒刀模价格不合法，仅能精确到小数点后两位！');
                    return;
                }

                var requestParam = {
                    "price": parseFloat($m_ui_input_price.val().trim()),
                    "boxprice": parseFloat($m_ui_input_price_box.val().trim())
                };

                $.ajax({    
                    type: "POST",
                    url: "diecutter-setprice.zc",
                    cache:  false,
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function (result)  {     //封装返回数据    
                        toastr.success('新刀模单价设置成功');
                        getPrice();
                    },
                    error: function() {
                        toastr.error('新刀模单价设置失败,请刷新重试！');
                    }  
                });  
            });
        }

        return wnd;
    }
};

var g_oDieCutterConfig = UIDieCutterConfig.createNew();
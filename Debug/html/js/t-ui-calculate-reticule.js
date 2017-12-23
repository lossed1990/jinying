var UIReticuleCalculate = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');

        var $m_ui_btn_calculate = null; //计算按钮
        var $m_ui_select_source = null; //材质下拉框
        var $m_ui_label_presstype = null; //材质标签，用于隐藏
        var $m_ui_select_presstype = null; //印刷方式下拉框
        var $m_ui_tab_parttype = null; //配件类型tab
        var $m_ui_tab_partcontent = null; //配件类型tab内容区域

        var $m_ui_table_selectpart = null; //所选配件表格

        var $m_ui_input_len = null;
        var $m_ui_input_width = null;
        var $m_ui_input_height = null;
        var $m_ui_input_count = null;
        var $m_ui_check_is_film = null;
        var $m_ui_check_is_press = null;

        var m_dataSet = new Array(); //选纸配件

        var $m_ui_input_customer = null;
        var $m_ui_input_finalprice = null;
        var $m_ui_btn_save = null;

        //页面参数加载时，请求的数据缓存 
        var m_param_sourcetype = null;
        var m_param_presstype = null;
        var m_param_parts = [];

        //=====公共方法=====
        wnd.load = function() {
            loadUI();
        }

        wnd.loadByParam = function(param){
            loadUI();
            var paramJson = JSON.parse(param); 

            $m_ui_input_count.val(paramJson.count);
            $m_ui_input_len.val(paramJson.source[0].l);
            $m_ui_input_width.val(paramJson.source[0].w);
            $m_ui_input_height.val(paramJson.source[0].h);
           
            if(paramJson.source[0].bfilm){
                $m_ui_check_is_film.iCheck('check');
            }else{
                $m_ui_check_is_film.iCheck('uncheck');
            }
            if(paramJson.source[0].bpress){
                $m_ui_check_is_press.iCheck('check');
            }else{
                $m_ui_check_is_press.iCheck('uncheck');
            }

            //缓存参数，以防异步加载时设置值被重置
            m_param_sourcetype = paramJson.source[0].sourcetype;
            m_param_presstype = paramJson.source[0].presstype;

            //加载配件
            m_param_parts = paramJson.parts;
        }

        //加载用户配置界面
        function loadUI() {
            var html_str =
                '<div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>手提袋报价 <small>功能描述：此处通过输入条件，计算手提袋的报价。</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">长(mm)</label>\
                                <div class="col-md-2 col-sm-2 col-xs-12">\
                                    <input id="input_len" type="text" class="form-control" placeholder="">\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">宽(mm)</label>\
                                <div class="col-md-2 col-sm-2 col-xs-12">\
                                    <input id="input_width" type="text" class="form-control" placeholder="">\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">高(mm)</label>\
                                <div class="col-md-2 col-sm-2 col-xs-12">\
                                    <input id="input_height" type="text" class="form-control" placeholder="">\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">数量(个)</label>\
                                <div class="col-md-2 col-sm-2 col-xs-12">\
                                    <input id="input_count" type="text" class="form-control" placeholder="">\
                                </div>\
                            </div>\
                            <div class = "form-group" > \
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">制作材质</label>\
                                <div class="col-md-11 col-sm-11 col-xs-12">\
                                    <select id="select_source_type" class="form-control">\
                                    </select>\
                                </div>\
                            </div>\
                            <div class = "form-group" > \
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">是否覆膜</label>\
                                <div class="col-md-2 col-sm-2 col-xs-12">\
                                    <input class="check_green" type="checkbox" id="check_is_film" checked="" required />\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">是否印刷</label>\
                                <div class="col-md-2 col-sm-2 col-xs-12">\
                                    <input class="check_green" type="checkbox" id="check_is_press" checked="" required />\
                                </div>\
                                <label id="label_press_type" class="control-label col-md-1 col-sm-1 col-xs-12">印刷方式</label>\
                                <div class="col-md-5 col-sm-5 col-xs-12">\
                                    <select id="select_press_type" class="form-control">\
                                    </select>\
                                </div>\
                            </div>\
                            <div class = "form-group" > \
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">选择配件</label>\
                                <div class="col-md-11 col-sm-11 col-xs-12">\
                                    <div class="tabbable tabs-below">\
                                        <ul class="nav nav-tabs" id="tab_part_type">\
                                            <li class="active"><a href="#测试" data-toggle="tab">Section 1</a></li>\
                                            <li><a href="#btab2" data-toggle="tab">Section 2</a></li>\
                                            <li><a href="#btab3" data-toggle="tab">Section 3</a></li>\
                                            <li><a href="#btab4" data-toggle="tab">Section 4</a></li>\
                                            <li><a href="#btab5" data-toggle="tab">Section 5</a></li>\
                                            <li><a href="#btab6" data-toggle="tab">Section 6</a></li>\
                                            <li><a href="#btab7" data-toggle="tab">Section 7</a></li>\
                                            <li><a href="#btab8" data-toggle="tab">Section 8</a></li>\
                                            <li><a href="#btab9" data-toggle="tab">Section 9</a></li>\
                                       </ul>\
                                       <div class="tab-content form-group" id="tab_part_content">\
                                            <div class="tab-pane active" id="测试">\
                                                <input class="check_aero" type="checkbox" checked="checked"> 蝴蝶结1\
                                                <input class="check_aero" type="checkbox" checked="checked"> 蝴蝶结2\
                                                <input class="check_aero" type="checkbox" class="flat" checked="checked"> 蝴蝶结3\
                                            </div>\
                                            <div class="tab-pane" id="btab2">\
                                              <p>Howdy, Im in Section 2.</p>\
                                            </div>\
                                            <div class="tab-pane" id="btab3">\
                                              <p>Howdy, Im in Section 3.</p>\
                                            </div>\
                                            <div class="tab-pane" id="btab4">\
                                              <p>Howdy, Im in Section 4.</p>\
                                            </div>\
                                            <div class="tab-pane" id="btab5">\
                                              <p>Howdy, Im in Section 5.</p>\
                                            </div>\
                                            <div class="tab-pane" id="btab6">\
                                              <p>Howdy, Im in Section 6.</p>\
                                            </div>\
                                            <div class="tab-pane" id="btab7">\
                                              <p>Howdy, Im in Section 7.</p>\
                                            </div>\
                                            <div class="tab-pane" id="btab8">\
                                              <p>Howdy, m in Section 8.</p>\
                                            </div>\
                                            <div class="tab-pane" id="btab9">\
                                              <p>Howdy, Im in Section 9.</p>\
                                            </div>\
                                        </div>\
                                    </div>\
                                    <br\>\
                                    <br\>\
                                    <table id="selectpart_table" class="table table-striped table-bordered">\
                                    <thead>\
                                        <tr>\
                                            <th>配件名称</th>\
                                            <th>配件类别</th>\
                                        </tr>\
                                    </thead>\
                                    <tbody>\
                                    </tbody>\
                                </table>\
                                </div>\
                            </div>\
                            </div>\
                            <div class = "ln_solid" > \
                            </div>\
                            <div class="form-horizontal form-label-left">\
                                <div class = "form-group" > \
                                    <div class = "col-md-5 col-sm-5 col-xs-12 col-md-offset-1" >\
                                        <button id = "submit_count" class = "btn btn-danger" >&nbsp;&nbsp;&nbsp;&nbsp;计&nbsp;&nbsp;算&nbsp;&nbsp;&nbsp;&nbsp;</button>\
                                    </div > \
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">总价</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_total_price" type="text" class="form-control" placeholder="" readonly>\
                                    </div>\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">单价</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_single_price" type="text" class="form-control" placeholder="" readonly>\
                                    </div>\
                                </div>\
                                <div class = "form-group" > \
                                    <div class = "col-md-11 col-sm-11 col-xs-12 col-md-offset-1" >\
                                        <table id="log_table" class="table table-striped table-bordered">\
                                            <thead>\
                                                <tr>\
                                                    <th>类型</th>\
                                                    <th>日志</th>\
                                                </tr>\
                                            </thead>\
                                        </table>\
                                    </div > \
                                </div>\
                            </div>\
                        </div > \
                    </div>\
                </div>\
                </div>\
                <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                <div class="x_title">\
                    <h2>保存订单 <small>功能描述：保存订单客户信息，用于后续查询与下单。</small></h2>\
                    <div class="clearfix"></div>\
                </div>\
                <div class="x_content">\
                    <div class="form-horizontal form-label-left">\
                        <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">客户名称</label>\
                            <div class="col-md-8 col-sm-8 col-xs-12">\
                                <input id="input_customer" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">最终总价</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_finalprice" type="text" class="form-control" placeholder="">\
                            </div>\
                        </div>\
                    </div>\
                    <div class = "ln_solid" > \
                    </div>\
                    <div class="form-horizontal form-label-left">\
                        <div class = "form-group" > \
                            <div class = "col-md-5 col-sm-5 col-xs-12 col-md-offset-1" >\
                                <button id = "submit_save" class = "btn btn-danger" >&nbsp;&nbsp;&nbsp;&nbsp;保&nbsp;&nbsp;存&nbsp;&nbsp;&nbsp;&nbsp;</button>\
                            </div > \
                        </div>\
                    </div>\
                </div > \
            </div>\
            </div>\
            </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            refreshSourceType();
            refreshPressType();
            refreshPartType();
            bingCalculatePrice();
            bingSaveOrder();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_btn_calculate = $('#submit_count');

            $m_ui_select_source = $('#select_source_type');
            $m_ui_label_presstype = $('#label_press_type');
            $m_ui_select_presstype = $('#select_press_type');
            $m_ui_tab_parttype = $('#tab_part_type');
            $m_ui_tab_partcontent = $('#tab_part_content');
            $m_ui_table_selectpart = $('#selectpart_table');

            $m_ui_input_len = $('#input_len');
            $m_ui_input_width = $('#input_width');
            $m_ui_input_height = $('#input_height');
            $m_ui_input_count = $('#input_count');
            $m_ui_check_is_film = $('#check_is_film');
            $m_ui_check_is_press = $('#check_is_press');

            $m_ui_input_customer = $('#input_customer');
            $m_ui_input_finalprice = $('#input_finalprice');
            $m_ui_btn_save = $('#submit_save');

            $('#check_is_press').on('ifChecked', function(event) {
                $m_ui_label_presstype.show();
                $m_ui_select_presstype.show();
            }).on('ifUnchecked', function(event) {
                $m_ui_label_presstype.hide();
                $m_ui_select_presstype.hide();
            });

            $('.check_green').iCheck({
                checkboxClass: 'icheckbox_flat-green'
            });

            $(".icheckbox_flat-green").css("margin-top", "8px");
            $m_ui_btn_save.prop('disabled', true);
            m_dataSet.length = 0;
        }

        //刷新配件种类选择控件
        function refreshTabdrop() {
            $('.nav-pills, .nav-tabs').tabdrop();
        }

        //刷新加载的配件元素
        function refreshPartCheckUI() {
            $('.check_aero').iCheck({
                checkboxClass: 'icheckbox_flat-aero'
            });

            $('.check_aero').on('ifChecked', function(event) {
                var subData = new Array();
                subData.push(event.target.name);
                subData.push(event.target.alt);
                m_dataSet.push(subData);
                $m_ui_table_selectpart.DataTable({
                    destroy: true, //可重新加载
                    data: m_dataSet
                });

            }).on('ifUnchecked', function(event) {
                for (var i = 0; i < m_dataSet.length; i++) {
                    if (m_dataSet[i][0] == event.target.name && m_dataSet[i][1] == event.target.alt) {
                        m_dataSet.splice(i, 1);
                        break;
                    }
                }

                $m_ui_table_selectpart.DataTable({
                    destroy: true, //可重新加载
                    data: m_dataSet
                });
            });

            //遍历刷新状态
            if(m_param_parts.length > 0)
            {
                var checks = $('.check_aero');
                checks.each(function(i,check){
                    if(m_param_parts.indexOf(check.name) > -1){
                        $(check).iCheck('check');
                    }
                });
            } 
        }

        function refreshSourceType() {
            $.ajax({    
                type: "POST",
                url: "sourcetype-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        $m_ui_select_source.html("");
                        toastr.error('暂未配置制作材质信息，请前往配置！');
                        return;
                    }

                    var strHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        var strSubHtml = '<option>' + result.data[i].name + '</option>';
                        strHtml = strHtml + strSubHtml;
                    }
                    $m_ui_select_source.html(strHtml);

                    if(m_param_sourcetype != null){
                        $m_ui_select_source.val(m_param_sourcetype);
                    }
                },
                error: function() {
                    $m_ui_select_source.html("");
                    toastr.error('获取制作材质信息失败,请刷新重试！');
                }  
            }); 
        }

        function refreshPressType() {
            $.ajax({    
                type: "POST",
                url: "presstype-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        $m_ui_select_presstype.html("");
                        toastr.error('暂未配置印刷方式信息，请前往配置！');
                        return;
                    }

                    var strHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        var strSubHtml = '<option>' + result.data[i].name + '</option>';
                        strHtml = strHtml + strSubHtml;
                    }
                    $m_ui_select_presstype.html(strHtml);

                    if(m_param_sourcetype != null){
                        $m_ui_select_presstype.val(m_param_presstype);
                    }
                },
                error: function() {
                    $m_ui_select_presstype.html("");
                    toastr.error('获取印刷方式信息失败,请刷新重试！');
                }  
            }); 
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
                        $m_ui_tab_parttype.html("");
                        $m_ui_tab_partcontent.html("");
                        toastr.error('暂未配置配件种类信息，请前往配置！');
                        return;
                    }

                    var strPartTypeHtml = '';
                    var strPartContentHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        if (i == 0) {
                            var strSubTypeHtml = "<li class='active'><a href='#" + result.data[i].name + "' data-toggle='tab'>" + result.data[i].name + '</a></li>';
                            var strSubContentHtml = "<div class='tab-pane active' id='" + result.data[i].name + "'></div>";

                        } else {
                            var strSubTypeHtml = "<li><a href='#" + result.data[i].name + "' data-toggle='tab'>" + result.data[i].name + '</a></li>';
                            var strSubContentHtml = "<div class='tab-pane' id='" + result.data[i].name + "'></div>";

                        }
                        strPartTypeHtml = strPartTypeHtml + strSubTypeHtml;
                        strPartContentHtml = strPartContentHtml + strSubContentHtml;
                    }
                    $m_ui_tab_parttype.html(strPartTypeHtml);
                    $m_ui_tab_partcontent.html(strPartContentHtml);

                    refreshTabdrop();
                    refreshPart();
                },
                error: function() {
                    $m_ui_tab_parttype.html("");
                    $m_ui_tab_partcontent.html("");
                    toastr.error('获取配件种类信息失败,请刷新重试！');
                }  
            });  
        }

        function refreshPart() {
            $.ajax({    
                type: "POST",
                url: "part-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        toastr.error('暂未配置配件信息，可前往配置！');
                        return;
                    }

                    for (var i = 0; i < result.data.length; i++) {
                        $('#' + result.data[i].type).append("<input class='check_aero' type='checkbox' name='" + result.data[i].name + "' alt='" + result.data[i].type + "' >&nbsp;" + result.data[i].name + "&nbsp;&nbsp;&nbsp;");
                    }

                    refreshPartCheckUI();
                },
                error: function() {
                    toastr.error('获取配件信息失败,请刷新重试！');
                }  
            });  
        }

        function bingCalculatePrice() {
            $m_ui_btn_calculate.on('click', function() {
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

                if (isNull($m_ui_input_count.val().trim())) {
                    toastr.error('数量不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_count.val().trim())) {
                    toastr.error('数量仅能输入数字！');
                    return;
                }

                var partsArray = new Array();
                for (var i = 0; i < m_dataSet.length; i++) {
                    partsArray.push(m_dataSet[i][0]);
                }

                var requestParam = {
                    count: parseInt($m_ui_input_count.val().trim()),
                    source: [{
                        l: parseInt($m_ui_input_len.val().trim()),
                        w: parseInt($m_ui_input_width.val().trim()),
                        h: parseInt($m_ui_input_height.val().trim()),
                        producttype: 0,
                        sourcetype: $m_ui_select_source.val().trim(),
                        sourceusetype: 0, //灰板、包纸
                        btangjin:false,
                        bfilm: $m_ui_check_is_film.is(':checked'),
                        bpress: $m_ui_check_is_press.is(':checked'),
                        presstype: $m_ui_select_presstype.val().trim(),
                    }],
                    parts: partsArray
                };

                $.ajax({    
                    type: "POST",
                    url: "calculate-price.zc",
                    cache:  false,
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function (result)  {     //封装返回数据    
                        $('#input_total_price').val(result.totalprice);
                        $('#input_single_price').val(result.price);

                        $('#log_table').DataTable({
                            "ordering": false, // 禁止排序
                            destroy: true, //可重新加载
                            data: result.log,
                            "createdRow": function(row, data, dataIndex) {    
                                if (data[0] == "错误") {
                                    $('td', row).css('color', '#f00')
                                } else if(data[0] == "需求"){
                                    $('td', row).css('color', '#8080C0')
                                } else if(data[0] == "材料"){
                                    $('td', row).css('color', '#00B271')
                                } else if(data[0] == "印刷"){
                                    $('td', row).css('color', '#479AC7') 
                                } else if(data[0] == "覆膜"){
                                    $('td', row).css('color', '#B45B3E')
                                } else if(data[0] == "压痕"){
                                    $('td', row).css('color', '#6699CC')
                                }   
                            }
                        });

                        $m_ui_btn_save.removeAttr('disabled');
                    },
                    error: function() {
                        toastr.error('向服务器请求失败,请稍后重试！');
                    }  
                });  
            });
        }

        function bingSaveOrder() {
            $m_ui_btn_save.on('click', function() {
                if (isNull($m_ui_input_customer.val().trim())) {
                    toastr.error('客户名称不能为空！');
                    return;
                }

                if (isNull($m_ui_input_finalprice.val().trim())) {
                    //toastr.error('最终总价不能为空！');
                    //return;
                    $m_ui_input_finalprice.val('0.00');
                }else{
                    if (!isPrice($m_ui_input_finalprice.val().trim())) {
                        toastr.error('最终总价输入的价格值不合法，仅能精确到小数点后两位！');
                        return;
                    }
                }

                var partsArray = new Array();
                for (var i = 0; i < m_dataSet.length; i++) {
                    partsArray.push(m_dataSet[i][0]);
                }

                var conditionParam = {
                    count: parseInt($m_ui_input_count.val().trim()),
                    source: [{
                        l: parseInt($m_ui_input_len.val().trim()),
                        w: parseInt($m_ui_input_width.val().trim()),
                        h: parseInt($m_ui_input_height.val().trim()),
                        producttype: 0,
                        sourcetype: $m_ui_select_source.val().trim(),
                        sourceusetype: 0, //灰板、包纸
                        btangjin:false,
                        bfilm: $m_ui_check_is_film.is(':checked'),
                        bpress: $m_ui_check_is_press.is(':checked'),
                        presstype: $m_ui_select_presstype.val().trim(),
                    }],
                    parts: partsArray
                };

                var requestParam = {
                    customer:$m_ui_input_customer.val().trim(),
                    user: $('#welcome_tip').html().trim(),
                    price: parseFloat($('#input_total_price').val().trim()),
                    finalprice:parseFloat($m_ui_input_finalprice.val().trim()),
                    condition:conditionParam
                };

                $.ajax({    
                    type: "POST",
                    url: "order-add.zc",
                    cache:  false,
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function (result)  {     //封装返回数据    
                        toastr.success('订单数据保存成功');
                        $m_ui_btn_save.prop('disabled', true);;
                    },
                    error: function() {
                        toastr.error('向服务器请求失败,请稍后重试！');
                    }  
                });  
            });
        }
       
        return wnd;
    }
};

var g_oReticuleCalculate = UIReticuleCalculate.createNew();
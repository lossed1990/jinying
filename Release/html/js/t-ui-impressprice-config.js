var UIImpressPriceConfig = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');
        var $m_ui_btn_setcheckprice = undefined;
        var $m_ui_btn_setprice = undefined;

        var $m_ui_input_checkprice = undefined;
        var $m_ui_input_price = undefined;

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
                        <h2>定价 <small>压痕校板费</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                        功能描述：此处用来设置压痕校板费。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">校板费(元)</label>\
                                <div class="col-md-11 col-sm-11 col-xs-12">\
                                    <input id="input_impress_checkprice" type="text" class="form-control" placeholder="">\
                                </div>\
                            </div>\
                            <div class="ln_solid">\
                                <div class="form-group">\
                                    <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                        <button id="submit_set_checkprice" class="btn btn-primary">设置</button>\
                                    </div>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>\
            <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>定价 <small>压痕单价</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：此处用来设置每件压痕费。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">单价(元)</label>\
                                <div class="col-md-11 col-sm-11 col-xs-12">\
                                    <input id="input_impress_price" type="text" class="form-control" placeholder="">\
                                </div>\
                            </div>\
                        <div class="ln_solid">\
                            <div class="form-group">\
                                <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                    <button id="submit_set_price" class="btn btn-primary">设置</button>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            bingCheckPriceEvent();
            bingPriceEvent();
            getCheckPrice();
            getPrice();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_btn_setcheckprice = $('#submit_set_checkprice');
            $m_ui_btn_setprice = $('#submit_set_price');
            $m_ui_input_checkprice = $('#input_impress_checkprice');
            $m_ui_input_price = $('#input_impress_price');
        }

        function getCheckPrice() {
            $.ajax({    
                type: "POST",
                url: "impress-getcheckprice.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    $m_ui_input_checkprice.val(result.checkprice);
                },
                error: function() {
                    toastr.error('获取压痕校板费失败,请刷新重试！');
                }  
            });  
        }

        function getPrice() {
            $.ajax({    
                type: "POST",
                url: "impress-getprice.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    $m_ui_input_price.val(result.price);
                },
                error: function() {
                    toastr.error('获取压痕单价失败,请刷新重试！');
                }  
            });  
        }

        function bingCheckPriceEvent() {
            $m_ui_btn_setcheckprice.on('click', function() {
                if (isNull($m_ui_input_checkprice.val().trim())) {
                    toastr.error('压痕校板费不能为空！');
                    return;
                }

                if (!isPrice($m_ui_input_checkprice.val().trim())) {
                    toastr.error('压痕校板费输入不合法，仅能精确到小数点后两位！');
                    return;
                }

                var requestParam = {
                    "checkprice": parseFloat($m_ui_input_checkprice.val().trim())
                };

                $.ajax({    
                    type: "POST",
                    url: "impress-setcheckprice.zc",
                    cache:  false,
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function (result)  {     //封装返回数据    
                        toastr.success('新压痕校板费设置成功');
                        getCheckPrice();
                    },
                    error: function() {
                        toastr.error('新压痕校板费设置失败,请刷新重试！');
                    }  
                });  
            });
        }

        function bingPriceEvent() {
            $m_ui_btn_setprice.on('click', function() {
                if (isNull($m_ui_input_price.val().trim())) {
                    toastr.error('压痕单价不能为空！');
                    return;
                }

                if (!isPrice($m_ui_input_price.val().trim())) {
                    toastr.error('压痕单价输入不合法，仅能精确到小数点后两位！');
                    return;
                }

                var requestParam = {
                    "price": parseFloat($m_ui_input_price.val().trim())
                };

                $.ajax({    
                    type: "POST",
                    url: "impress-setprice.zc",
                    cache:  false,
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function (result)  {     //封装返回数据    
                        toastr.success('新压痕单价设置成功');
                        getPrice();
                    },
                    error: function() {
                        toastr.error('新压痕单价设置失败,请刷新重试！');
                    }  
                });  
            });
        }

        return wnd;
    }
};

var g_oImpressPriceConfig = UIImpressPriceConfig.createNew();
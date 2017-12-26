var TCommonFunction = {
    createNew: function() {
        var obj = {};
        //保存订单
        obj.ajaxSaveOrder = function(requestParam, successcallback) {
            requestParam.user = $('#welcome_tip').html().trim();

            $.ajax({    
                type: "POST",
                url: "order-add.zc",
                cache:  false,
                data: JSON.stringify(requestParam),
                dataType: "json",
                success: function (result)  {     //封装返回数据
                    if (result.ok == 0) {
                        toastr.success('订单数据保存成功');
                        if (typeof successcallback === "function") {
                            successcallback();
                        }
                    } else {
                        toastr.error(result.errorinfo);
                    }    
                },
                error: function() {
                    toastr.error('向服务器请求失败,请稍后重试！');
                }  
            });
        }

        return obj;
    }
};

var g_common = TCommonFunction.createNew();
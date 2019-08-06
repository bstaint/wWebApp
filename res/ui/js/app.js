var app = new Vue({
    el: '#app',

    data: {
        content: "",
        activeIndex2: '1'
    },

    methods: {
        handleSelect(key, keyPath) {
            console.log(key, keyPath);
        },
        handleOpen(key, keyPath) {
            console.log(key, keyPath);
        },
        handleClose(key, keyPath) {
            console.log(key, keyPath);
        },

        onSysTitleMove(ev) {
            if(ev.target.tagName == "UL")
                exe("mousedown")
        }
    },

    components: {
        'titlebar': {
            template: '#titlebar',
            methods: {}
        }
    }
});

document.body.parentNode.style.overflow = "hidden";
var app = new Vue({
    el: '#app',

    data: {
        content: "",
        activePage: '1'
    },

    methods: {
        handleSelect(key, keyPath) {
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
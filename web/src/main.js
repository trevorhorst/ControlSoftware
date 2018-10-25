import React from 'react'

class Main extends React.Component {

    constructor( props ) {
        super( props );
    }

    componentWillMount() {
        // injectTapEventPlugin();
    }

    render() {
        return (
            <div>
                Hello World!
            </div>
        );
    }
}

export default Main;

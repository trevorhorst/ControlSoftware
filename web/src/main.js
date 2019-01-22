import React from 'react'
import API from './api/api.js'

class Main extends React.Component {

    constructor( props ) {
        super( props );

        let comp = this
        let qchrono = {}
        let datetime = 'TIME'

        this.state = {
            qchrono: qchrono
            , datetime: datetime
            , setTimes: 0
        }

        this.tick = this.tick.bind(this);
    }

    tick() {
        let comp = this;
        let res = "";
        API.sendCommand( {"cmd":"qchrono"} )
        .then( function( result ) {
            comp.setState( {
                qchrono: result.result
            } )
            res = result.result.datetime
        } )
    }

    componentWillMount() {
        // injectTapEventPlugin();
    }

    componentDidMount() {
        this.interval = setInterval( this.tick, 500 );
    }

    componentWillUnmount() {
        clearInterval( this.interval );
    }

    render() {
        let comp = this
        return (
            <div>
                {comp.state.qchrono.datetime}
            </div>
        );
    }
}

export default Main;

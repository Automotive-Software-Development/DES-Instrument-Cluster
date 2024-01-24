# Instrument Cluster Qt Application

### Overview
#### This Qt application serves as an instrument cluster for monitoring and displaying real-time data such as revolutions per second (RPS) and battery level. It interacts with D-Bus to retrieve data from external sources.

### C++ Integration

- #### Define Q_PROPERTY macro to expose the attributes of C++ type to Qml
    ```
    Q_PROPERTY(float rps READ rps NOTIFY rpsChanged FINAL)
    Q_PROPERTY(double battery READ battery NOTIFY batteryChanged FINAL)
    ```

- #### Define signals and slots
    ```
    public slots:
        float rps();
        double battery();

    signals:
        void rpsChanged();
        void batteryChanged();
    ```

- #### Create private variables
    ```
    private:
        float m_rps;
        double m_battery;
    ```
    > **Why Private Variables?**
    - Encapsulation in object-oriented programming involves bundling data and methods within a class. Private variables, like `m_rps`, play a crucial role for the following reasons:

    - *Encapsulation(Data Hiding):* Private variables hide the internal state of an object, preventing external code from directly modifying it and ensuring the object remains in a valid and consistent state.

- #### Connect Signals and Slots
    ```
    QObject::connect(&obj, SIGNAL(rpsChanged()), &obj, SLOT(rps()));
    QObject::connect(&obj, SIGNAL(batteryChanged()), &obj, SLOT(battery()));
    ```

- #### Create receiver functions that will fetch the data from D-bus
    1. Connect to D-bus service using QDBusInterface
        ```
        QDBusInterface dbusInterface(<service_name>, <interface_name>, <path>, QDBusConnection::sessionBus());
        ```
    1. Call the method from the D-bus service
        ```
        QDBusReply<QString> rps = dbusInterface.call("RPS");
        ```
    1. Emit the according signal
        ```
        emit rpsChanged();
        ```
        > **Note: When the signal is emnitted, connected slot will be called and then the slot will update the private variable's value and that value will update public Q_PROPERTY value. That's how it'll reflect on the display**

### Connecting C++ to QML

In this project, C++ and QML communicate seamlessly through the use of the `QQmlContext` class. The key step is establishing a connection between C++ objects and QML by setting a context property.

```
QQmlContext *rootContext = engine.rootContext();
rootContext->setContextProperty("Instrument_Cluster", &obj);
```

### Qml Integration

The interaction between C++ and QML in this project is facilitated through Qt's signals and slots mechanism. The `DataReceiver` class exposes C++ properties and functions, which are then accessed and manipulated in QML.

- The `main.qml` file defines the graphical interface of the instrument cluster.
- The `CircularGauge` elements represent the speedometer and RPS gauge.
- The battery level is displayed using a `Rectangle` with a dynamic width.

The properties and functions of DataReceiver are accessed directly in QML. For example, the Instrument_Cluster.rps and Instrument_Cluster.battery properties are used in various elements.

```
// Speedometer
CircularGauge {
    // ...
    value: Instrument_Cluster.rps * 3.14 * 2.5
}

// Battery
Rectangle {
    // ...
    width: Instrument_Cluster.battery * 2.6
}

// RPS Gauge
CircularGauge {
    // ...
    value: Instrument_Cluster.rps
}

```


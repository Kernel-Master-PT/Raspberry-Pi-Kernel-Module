#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>

#define ARDUINO_TOGGLE_NR 20
#define INIT_SLEEP_TIME 10

struct arduino {
    struct platform_device *pdev;
    struct gpio_desc *gpio;
    int gpio_state;
};

static int arduino_probe(struct platform_device *pdev)
{
    int i = 0;
    struct device *dev = &pdev->dev;
    struct arduino *arduino;

    dev_info(dev, "arduino gpio cunsumer being probed\n");
    dev_info(dev, "waiting for %d secs...\n", INIT_SLEEP_TIME);

    msleep(INIT_SLEEP_TIME*1000);

    arduino = devm_kzalloc(&pdev->dev, sizeof(struct arduino), GFP_KERNEL);
    if (!arduino) {
        dev_err(dev, "failed to allocate memory");
        return -ENOMEM;
    }

    arduino->pdev = pdev;
    arduino->gpio_state = 0;
    platform_set_drvdata(pdev, arduino);

    arduino->gpio = devm_gpiod_get(dev, "arduino", GPIOD_OUT_LOW);
    if (IS_ERR(arduino->gpio)) {
        dev_err(dev, "failed to claim gpio: err=%ld\n", PTR_ERR(arduino->gpio));
        return PTR_ERR(arduino->gpio);
    }
    for(i = 0; i < ARDUINO_TOGGLE_NR; i++) {
        arduino->gpio_state = !arduino->gpio_state;
        gpiod_set_value(arduino->gpio, arduino->gpio_state);
        dev_info(dev, "gpio toggled. state=%d\n", arduino->gpio_state);
        msleep(500);
    }
    return 0;
};

static int arduino_remove(struct platform_device *pdev)
{
    struct arduino *arduino = platform_get_drvdata(pdev);
    dev_info(&pdev->dev, "removing with gpio_state=%d\n", arduino->gpio_state);
    return 0;
}

static struct of_device_id arduino_of_table[] = {
    {.compatible="arduino,uno"},
    {}
};
MODULE_DEVICE_TABLE(of, arduino_of_table);

static struct platform_driver arduino_drv = {
    .probe = arduino_probe,
    .remove = arduino_remove,
    .driver = {
        .name = KBUILD_MODNAME,
        .owner = THIS_MODULE,
        .of_match_table = arduino_of_table,
    },
};

module_platform_driver(arduino_drv);
MODULE_LICENSE("GPL");

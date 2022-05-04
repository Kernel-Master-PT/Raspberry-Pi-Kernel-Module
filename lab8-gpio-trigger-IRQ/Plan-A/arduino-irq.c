#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

struct arduino_irq_dev {
    struct platform_device *pdev;
    struct gpio_desc *gpiod;
    int irq;
    struct work_struct work;
};

static void irq_bottom_half(struct work_struct *work)
{
    struct arduino_irq_dev *arduino;
    arduino = container_of(work, struct arduino_irq_dev, work);
    pr_info("irq=%d bottom half executed\n", arduino->irq);
    return;
};

static irqreturn_t irq_top_half(int irq, void *p)
{
    struct platform_device *pdev = p;
    struct arduino_irq_dev *arduino;

    arduino = platform_get_drvdata(pdev);
    schedule_work(&arduino->work);
    return IRQ_HANDLED;
}

static int arduino_probe(struct platform_device *pdev)
{
    struct device *dev = &(pdev->dev);
    struct arduino_irq_dev *arduino;

    int ret = 0;

    arduino = devm_kzalloc(dev, sizeof(struct arduino_irq_dev), GFP_KERNEL);
    if (!arduino) {
        dev_err(dev, "failed to allocate memory");
        return -ENOMEM;
    }

    arduino->pdev = pdev;
    platform_set_drvdata(pdev, arduino);

    arduino->gpiod = devm_gpiod_get_index(dev, "arduino", 0, GPIOD_IN);
    if (IS_ERR(arduino->gpiod)) {
        dev_err(dev, "failed to get irq\n");
        return PTR_ERR(arduino->gpiod);
    }

    INIT_WORK(&arduino->work, irq_bottom_half);

    arduino->irq = gpiod_to_irq(arduino->gpiod);
    if (arduino->irq < 0) {
        dev_err(dev, "failed to get irq from gpiod\n");
        return ret;
    }

    ret = devm_request_irq(dev, arduino->irq, irq_top_half, IRQF_TRIGGER_RISING, dev_name(&pdev->dev), pdev);
    if (ret < 0) {
        dev_err(dev, "failed to request IRQ.\n");
        return ret;
    }

    dev_info(dev, "Successfully probed arduino!\n");
    return 0;
};

static const struct of_device_id arduino_ids[] = {
    {.compatible="arduino,uno-irq",},
    {}
};

static struct platform_driver arduino_irq_driver = {
    .driver = {
        .name = "arduino-irq",
        .of_match_table = arduino_ids,
    },
    .probe = arduino_probe
};

MODULE_LICENSE("GPL");
module_platform_driver(arduino_irq_driver);

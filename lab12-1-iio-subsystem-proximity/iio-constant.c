#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/iio/consumer.h>
#include <linux/iio/iio.h>
#include <linux/of.h>

#define IIO_CHANNEL_DEFINE(num)   {\
    .type = IIO_PROXIMITY,\
    .indexed = 1,\
    .channel = (num),\
    .info_mask_separate = BIT(IIO_CHAN_INFO_PROCESSED),\
  }\

static const struct iio_chan_spec constant_iio_channels[] = {
  IIO_CHANNEL_DEFINE(0),
  IIO_CHANNEL_DEFINE(1),
  IIO_CHANNEL_DEFINE(2),
  IIO_CHANNEL_DEFINE(3),
  IIO_CHANNEL_DEFINE(4),
  IIO_CHANNEL_DEFINE(5),
};

static int constant_iio_read_raw(struct iio_dev *iio,
                                  struct iio_chan_spec const *chan,
                                  int *val0, int *val1, long mask)
{
  *val0 = 9876;
  *val1 = 1234;
  switch (chan->channel)
  {
    case 0:
      return IIO_VAL_INT;
    case 1:
      return IIO_VAL_INT_PLUS_MICRO;
    case 2:
      return IIO_VAL_INT_PLUS_MICRO_DB;
    case 3:
      return IIO_VAL_INT_MULTIPLE;
    case 4:
      return IIO_VAL_FRACTIONAL;
    case 5:
      return IIO_VAL_FRACTIONAL_LOG2;
    default:
      return 0;
  }
}

struct iio_info constant_iio_info = {
  .read_raw = constant_iio_read_raw,
};

static int constant_iio_probe(struct platform_device *pdev)
{
  struct device *dev = &pdev->dev;
  struct iio_dev *iio;

  dev_info(dev, "probing");

  iio = devm_iio_device_alloc(dev, 0);
  if (IS_ERR(iio)) {
    dev_err(dev, "failed to allocate iio device");
    return PTR_ERR(iio);
  }
  iio->name = pdev->name;
  iio->info = &constant_iio_info;
  iio->modes = INDIO_DIRECT_MODE;
  iio->channels = constant_iio_channels;
  iio->num_channels = ARRAY_SIZE(constant_iio_channels);

  return devm_iio_device_register(dev, iio);
}

static const struct of_device_id constant_iio_ids[] = {
  {.compatible = "iio,constant",},
  {}
};
static struct platform_driver constant_iio_driver = {
  .driver = {
    .name = KBUILD_MODNAME,
    .of_match_table = constant_iio_ids,
  },
  .probe = constant_iio_probe
};

MODULE_LICENSE("GPL");
module_platform_driver(constant_iio_driver);

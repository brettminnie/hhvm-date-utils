<?hh

namespace DateUtilsTest;

use DateUtils\BankHolidays;

class BankHolidaysTest extends \PHPUnit_Framework_TestCase
{
    protected $bankHolidays;

    public function setUp()
    {
        $configs        = include('config/module.config.php');

        $this->bankHolidays = new BankHolidays($configs, 2012);
    }

    public function testSetUpEmptyConfig()
    {
        $configs        = include('config/module.config.php');
        $bankHolidays = new BankHolidays($configs, 2000);
        $bankHolidays = $bankHolidays->getBankHolidays();

        $this->assertEquals('2000-01-01', $bankHolidays['newYearsDay']);
        $this->assertEquals('2000-12-25', $bankHolidays['xmasDay']);
    }

    public function testCalculateFixedHolidays()
    {
        $holidays = $this->bankHolidays->calculateFixedHolidays(2012);
        $this->assertTrue(is_array($holidays));

        $this->assertTrue(array_key_exists('newYearsDay', $holidays));
        $this->assertTrue(array_key_exists('goodFriday', $holidays));
        $this->assertTrue(array_key_exists('easterMonday', $holidays));
        $this->assertTrue(array_key_exists('earlyMay', $holidays));
    }

    public function testGetBankHolidays()
    {
        $holidays = $this->bankHolidays->getBankHolidays();
        $this->assertTrue(array_key_exists('queensDiamondJubilee', $holidays));
        $this->assertEquals("2012-02-01", $holidays['newYearsDay']);
    }

    public function testEasterDate()
    {
        $this->assertEquals($this->bankHolidays->easterDate(2000), mktime(0, 0, 0, 4, 23, 2000));
        $this->assertEquals($this->bankHolidays->easterDate(2005), mktime(0, 0, 0, 3, 27, 2005));
        $this->assertEquals($this->bankHolidays->easterDate(2010), mktime(0, 0, 0, 4, 4, 2010));
        $this->assertEquals($this->bankHolidays->easterDate(2015), mktime(0, 0, 0, 4, 5, 2015));
        $this->assertEquals($this->bankHolidays->easterDate(2020), mktime(0, 0, 0, 4, 12, 2020));
    }
}

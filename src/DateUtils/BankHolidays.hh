<?hh

namespace DateUtils;

/**
 * Class BankHolidays
 * @package DateUtils
 */
final class BankHolidays
{
    /**
     * @var int
     */
    protected int $year;

    /**
     * @var array
     */
    protected array<string> $bankHolidays;

    public function __construct(array $configArray, int $year)
    {
        $this->year = $year;

        $this->bankHolidays = (!empty($configArray['bankHolidays'][$year])) ?
                $configArray['bankHolidays'][$year] :
                array();

        $this->bankHolidays = array_merge(
              $this->calculateFixedHolidays($year),
              $this->bankHolidays
        );
    }

    /**
     * @param $year
     * @return array
     */
    public function calculateFixedHolidays(int $year): array<string>
    {
        $bankHolidays = array();
        $bankHolidays['newYearsDay'] = date('Y-m-d', strtotime('first day of january ' . $year));
        $bankHolidays['goodFriday'] = date('Y-m-d', strtotime('previous friday', $this->easterDate($year)));
        $bankHolidays['easterMonday'] = date('Y-m-d', strtotime('next monday', $this->easterDate($year)));
        $bankHolidays['earlyMay'] = date('Y-m-d', strtotime('first monday of may ' . $year));
        $bankHolidays['lastMay'] = date('Y-m-d', strtotime('last monday of may ' . $year));
        $bankHolidays['lateAugust'] = date('Y-m-d', strtotime('last monday of august ' . $year));
        $bankHolidays['xmasDay'] = date('Y-m-d', strtotime('25 december ' . $year));
        $bankHolidays['boxingDay'] = date('Y-m-d', strtotime('26 december ' . $year));

        return $bankHolidays;
    }

    /**
     * @return array
     */
    public function getBankHolidays() : array
    {
        return $this->bankHolidays;
    }

    /**
     * @param int $year
     * @return int
     */
    public function easterDate(int $year): int
    {
        $lunarAge = $goldenNumber = $century = $fullMoonOffset = 0;
        $weekday = $sundayOffset = $month = $day = $easterTimeStamp = 0;

        $goldenNumber = $year % 19;
        $century = (int)($year / 100);

        $lunarAge =
            (int)($century - (int)($century / 4) -
                (int)((8 * $century + 13) / 25) + 19 * $goldenNumber + 15) % 30;

        $fullMoonOffset =
            (int)$lunarAge -
            (int)($lunarAge / 28) *
            (int)(1 - (int)($lunarAge / 28) * (int)(29 / ($lunarAge + 1)) * ((int)(21 - $goldenNumber) / 11));

        $weekday = ($year + (int)($year / 4) + $fullMoonOffset + 2 - $century + (int)($century / 4)) % 7;

        $sundayOffset = $fullMoonOffset - $weekday;
        $month = 3 + (int)(($sundayOffset + 40) / 44);
        $day = $sundayOffset + 28 - 31 * ((int)($month / 4));

        $easterTimeStamp = mktime(0, 0, 0, $month, $day, $year);

        return $easterTimeStamp;
    }
}


import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

const sidebars: SidebarsConfig = {
  docsSidebar: [
    {
      type: 'category',
      label: '🚀 Getting Started',
      collapsed: false,
      items: ['intro', 'building', 'configuration'],
    },
    {
      type: 'category',
      label: '🎮 Usage',
      items: ['controls', 'wad-files'],
    },
    {
      type: 'category',
      label: '🔧 Development',
      items: ['porting', 'api', 'contributing'],
    },
  ],
};

export default sidebars;
